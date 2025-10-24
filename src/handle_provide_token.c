#include "apwine_plugin.h"

void handle_swap_exact_tokens(apwine_parameters_t *context) {
    contract_address_ticker_t *currentToken = NULL;

    context->decimals_sent = WEI_TO_ETHER;
    context->decimals_received = WEI_TO_ETHER;

    for (uint8_t i = 0; i < NUM_CONTRACT_ADDRESS_COLLECTION; i++) {
        currentToken = (contract_address_ticker_t *) PIC(&CONTRACT_ADDRESS_COLLECTION[i]);
        if (memcmp(currentToken->_amm, context->contract_address_sent, ADDRESS_LENGTH) == 0) {
            context->decimals_sent = currentToken->decimal;
            context->decimals_received = currentToken->decimal;
            break;
        }
    }
}

void handle_future_vault_tokens(apwine_parameters_t *context) {
    contract_address_future_vault_t *currentToken = NULL;

    context->decimals_sent = WEI_TO_ETHER;
    context->decimals_received = WEI_TO_ETHER;

    for (uint8_t i = 0; i < NUM_CONTRACT_ADDRESS_FUTURE_VAULT; i++) {
        currentToken = (contract_address_future_vault_t *) PIC(&CONTRACT_ADDRESS_FUTURE_VAULT[i]);
        if (memcmp(currentToken->_amm, context->contract_address_sent, ADDRESS_LENGTH) == 0) {
            context->decimals_sent = currentToken->decimal;
            context->decimals_received = currentToken->decimal;
            break;
        }
    }
}

void handle_liquidity_tokens(ethPluginProvideInfo_t *msg, apwine_parameters_t *context) {
    if (context->contract_sent_unknown) {
        default_sent_network_token(msg, context);
    } else {
        PRINTF("contract_sent_unknow not set\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
    }

    if (context->contract_received_unknown) {
        default_received_network_token(msg, context);
    } else {
        PRINTF("contract_received_unknow not set\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
    }
}

void handle_token(ethPluginProvideInfo_t *msg, apwine_parameters_t *context) {
    if (context->contract_sent_unknown) {
        default_sent_network_token(msg, context);
    } else if (msg->item1 != NULL) {
        context->decimals_sent = msg->item1->token.decimals;
        strlcpy(context->ticker_sent,
                (char *) msg->item1->token.ticker,
                sizeof(context->ticker_sent));
        context->tokens_found |= TOKEN_SENT_FOUND;
    } else {
        // CAL did not find the token and token is not ETH.
        default_sent_network_token(msg, context);
    }
}

void handle_provide_token(ethPluginProvideInfo_t *msg) {
    apwine_parameters_t *context = (apwine_parameters_t *) msg->pluginContext;

    switch (context->selectorIndex) {
        case SWAP_EXACT_AMOUNT_IN:
        case SWAP_EXACT_AMOUNT_OUT:
        case ZAPINTOPT:
            handle_swap_exact_tokens(context);
            break;
        case DEPOSIT:
        case WITHDRAW:
            handle_future_vault_tokens(context);
            break;
        case ADD_LIQUIDITY:
        case REMOVE_LIQUIDITY:
            handle_liquidity_tokens(msg, context);
            break;
        default:
            handle_token(msg, context);
            break;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}
