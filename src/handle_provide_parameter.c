#include "apwine_plugin.h"

// Store the amount sent in the form of a string, without any ticker or decimals. These will be
// added when displaying.
static void handle_amount_sent(ethPluginProvideParameter_t *msg, apwine_parameters_t *context) {
    memcpy(context->amount_sent, msg->parameter, INT256_LENGTH);
    printf_hex_array("AMOUNT SENT: ", INT256_LENGTH, context->amount_sent);
}

// Store the amount received in the form of a string, without any ticker or decimals. These will be
// added when displaying.
static void handle_amount_received(ethPluginProvideParameter_t *msg, apwine_parameters_t *context) {
    memcpy(context->amount_received, msg->parameter, PARAMETER_LENGTH);
    printf_hex_array("AMOUNT RECEIVED: ", PARAMETER_LENGTH, context->amount_received);
}

static void handle_token_sent(ethPluginProvideParameter_t *msg, apwine_parameters_t *context) {
    copy_address(context->contract_address_sent,
                 msg->parameter,
                 sizeof(context->contract_address_sent));
    printf_hex_array("TOKEN SENT: ", ADDRESS_LENGTH, context->contract_address_sent);
}

static void handle_swap_exact_amount(ethPluginProvideParameter_t *msg,
                                     apwine_parameters_t *context) {
    switch (context->next_param) {
        case TOKEN_SENT:  // _amm
            handle_token_sent(msg, context);
            context->next_param = AMOUNT_SENT;
            context->skip = 2;  // skip _pairPath and _tokenPath offset
            break;
        case AMOUNT_SENT:  // _tokenAmountIn
            handle_amount_sent(msg, context);
            context->next_param = AMOUNT_RECEIVED;
            break;
        case AMOUNT_RECEIVED:  // _minAmountOut
            handle_amount_received(msg, context);
            context->next_param = PAIR_PATH_LENGTH;
            context->skip = 3;  // skip _to, _deadline and _referralRecipient
            break;
        case PAIR_PATH_LENGTH:  // _pairPath length
            if (!U2BE_from_parameter(msg->parameter, &(context->array_len)) &&
                context->array_len == 0) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
            }
            context->tmp_len = context->array_len;
            context->next_param = PAIR_PATH_FIRST;
            break;
        case PAIR_PATH_FIRST:  // _pairPath[0]
            context->tmp_len--;

            if (!U2BE_from_parameter(msg->parameter, &context->pair_path_first)) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
            }

            if (context->tmp_len == 0) {
                context->next_param = TOKEN_PATH_LENGTH;
            } else {
                context->skip = context->tmp_len - 1;
                context->next_param = PAIR_PATH_LAST;
            }
            break;

        case PAIR_PATH_LAST:  // _pairPath[-1]
            if (!U2BE_from_parameter(msg->parameter, &context->pair_path_last)) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
            }
            context->next_param = TOKEN_PATH_LENGTH;
            break;

        case TOKEN_PATH_LENGTH:
            if (!U2BE_from_parameter(msg->parameter, &(context->tmp_len)) &&
                context->tmp_len != context->array_len * 2) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
            }
            context->next_param = TOKEN_PATH_SENT;
            break;
        case TOKEN_PATH_SENT:  // _tokenPath[0]
            if (!U2BE_from_parameter(msg->parameter, &(context->token_path_sent))) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
            }
            context->skip = context->tmp_len - 2;
            context->next_param = TOKEN_PATH_RECEIVED;
            break;

        case TOKEN_PATH_RECEIVED:  // _tokenPath[-1]
            if (!U2BE_from_parameter(msg->parameter, &(context->token_path_received))) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
            }
            context->valid = 1;
            context->next_param = ERROR_PARAM;
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_liquidity(ethPluginProvideParameter_t *msg, apwine_parameters_t *context) {
    switch (context->next_param) {
        case AMOUNT_SENT:  // _minAmountsIn[0] will be copied in AMOUNT_SENT
            handle_amount_sent(msg, context);
            context->contract_sent_unknown = 1;
            context->next_param = AMOUNT_RECEIVED;
            break;
        case AMOUNT_RECEIVED:  // _minAmountsIn[1]
            handle_amount_received(msg, context);
            context->contract_received_unknown = 1;
            context->valid = 1;
            context->next_param = ERROR_PARAM;
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_deposit_withdraw(ethPluginProvideParameter_t *msg,
                                    apwine_parameters_t *context) {
    switch (context->next_param) {
        case TOKEN_SENT:  // _futureVault
            handle_token_sent(msg, context);
            context->next_param = AMOUNT_SENT;
            break;
        case AMOUNT_SENT:  // _amount
            handle_amount_sent(msg, context);
            // When all parameters are parsed
            context->valid = 1;
            context->next_param = ERROR_PARAM;
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_zapintopt(ethPluginProvideParameter_t *msg, apwine_parameters_t *context) {
    switch (context->next_param) {
        case TOKEN_SENT:  // _amm
            handle_token_sent(msg, context);
            context->next_param = AMOUNT_SENT;
            break;
        case AMOUNT_SENT:  // _amount
            handle_amount_sent(msg, context);
            context->next_param = INPUT_LENGTH;
            context->skip = 2;
            break;
        case INPUT_LENGTH:  // _inputs length
            if (!U2BE_from_parameter(msg->parameter, &(context->array_len))) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
            }
            if (context->array_len == 0) {
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
            }
            context->next_param = AMOUNT_RECEIVED;
            break;
        case AMOUNT_RECEIVED:  // _inputs[0]
            handle_amount_received(msg, context);
            context->skip = context->array_len - 1;
            // When all parameters are parsed
            context->valid = 1;
            context->next_param = ERROR_PARAM;
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_increase_amount(ethPluginProvideParameter_t *msg, apwine_parameters_t *context) {
    switch (context->next_param) {
        case AMOUNT_SENT:  // _value
            handle_amount_sent(msg, context);
            context->contract_sent_unknown = 1;
            // When all parameters are parsed
            context->valid = 1;
            context->next_param = ERROR_PARAM;
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_create_lock(ethPluginProvideParameter_t *msg, apwine_parameters_t *context) {
    switch (context->next_param) {
        case AMOUNT_SENT:  // _value
            handle_amount_sent(msg, context);
            context->contract_sent_unknown = 1;
            context->next_param = AMOUNT_RECEIVED;
            break;
        case AMOUNT_RECEIVED:  // _unlock_time
            handle_amount_received(msg, context);
            // When all parameters are parsed
            context->valid = 1;
            context->next_param = ERROR_PARAM;
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_increase_unlock_time(ethPluginProvideParameter_t *msg,
                                        apwine_parameters_t *context) {
    switch (context->next_param) {
        case AMOUNT_SENT:  // _unlock_time
            handle_amount_sent(msg, context);
            // When all parameters are parsed
            context->valid = 1;
            context->next_param = ERROR_PARAM;
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

void handle_provide_parameter(ethPluginProvideParameter_t *msg) {
    apwine_parameters_t *context = (apwine_parameters_t *) msg->pluginContext;
    printf_hex_array("apwine plugin provide parameter: ", PARAMETER_LENGTH, msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;

    if (context->skip) {
        // Skip this step, and don't forget to decrease skipping counter.
        context->skip--;
    } else {
        switch (context->selectorIndex) {
            case SWAP_EXACT_AMOUNT_IN:
            case SWAP_EXACT_AMOUNT_OUT:
                handle_swap_exact_amount(msg, context);
                break;
            case REMOVE_LIQUIDITY:
            case ADD_LIQUIDITY:
                handle_liquidity(msg, context);
                break;
            case DEPOSIT:
            case WITHDRAW:
                handle_deposit_withdraw(msg, context);
                break;
            case ZAPINTOPT:
                handle_zapintopt(msg, context);
                break;
            case INCREASE_AMOUNT:
                handle_increase_amount(msg, context);
                break;
            case CREATE_LOCK:
                handle_create_lock(msg, context);
                break;
            case INCREASE_UNLOCK_TIME:
                handle_increase_unlock_time(msg, context);
                break;
            case REDEEM_YIELD:
                // When all parameters are parsed
                context->valid = 1;
                context->next_param = ERROR_PARAM;
                break;
            default:
                PRINTF("Selector Index %d not supported\n", context->selectorIndex);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
        }
    }
}
