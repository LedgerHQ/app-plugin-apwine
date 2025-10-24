#include "apwine_plugin.h"

// Called once to init.
void handle_init_contract(ethPluginInitContract_t *msg) {
    if (msg->interfaceVersion != ETH_PLUGIN_INTERFACE_VERSION_LATEST) {
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    if (msg->pluginContextLength < sizeof(apwine_parameters_t)) {
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    apwine_parameters_t *context = (apwine_parameters_t *) msg->pluginContext;
    memset(context, 0, sizeof(*context));
    context->valid = 0;

    // Determine a function to call
    size_t i;
    for (i = 0; i < NUM_APWINE_SELECTORS; i++) {
        if (memcmp((uint8_t *) PIC(APWINE_SELECTORS[i]), msg->selector, SELECTOR_SIZE) == 0) {
            context->selectorIndex = i;
            break;
        }
    }

    if (i == NUM_APWINE_SELECTORS) {
        // Selector was not found
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    // Set `next_param` to be the first field we expect to parse.
    switch (context->selectorIndex) {
        case SWAP_EXACT_AMOUNT_IN:
        case SWAP_EXACT_AMOUNT_OUT:
        case DEPOSIT:
        case WITHDRAW:
            context->next_param = TOKEN_SENT;
            break;
        case ADD_LIQUIDITY:
        case REMOVE_LIQUIDITY:
            context->skip = 2;
            context->next_param = AMOUNT_SENT;
            break;
        case ZAPINTOPT:
            context->next_param = TOKEN_SENT;
            break;
        case REDEEM_YIELD:
        case INCREASE_AMOUNT:
        case CREATE_LOCK:
        case INCREASE_UNLOCK_TIME:
            context->next_param = AMOUNT_SENT;
            break;
        default:
            PRINTF("Missing selectorIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}
