#include "apwine_plugin.h"

void handle_finalize(ethPluginFinalize_t *msg) {
    apwine_parameters_t *context = (apwine_parameters_t *) msg->pluginContext;
    if (context->valid) {
        switch (context->selectorIndex) {
            case REDEEM_YIELD:
            case INCREASE_AMOUNT:
            case INCREASE_UNLOCK_TIME:
                msg->numScreens = 1;
                break;
            case SWAP_EXACT_AMOUNT_IN:
            case SWAP_EXACT_AMOUNT_OUT:
            case ZAPINTOPT:
                msg->numScreens = 4;
                break;
            default:
                msg->numScreens = 2;
                break;
        }

        if (!ADDRESS_IS_NETWORK_TOKEN(context->contract_address_sent) ||
            context->contract_sent_unknown) {
            // Address is not network token (0xeee...) so we will need to look up the token in the
            // CAL.
            printf_hex_array("Setting address sent to: ",
                             ADDRESS_LENGTH,
                             context->contract_address_sent);
            msg->tokenLookup1 = context->contract_address_sent;
        } else {
            sent_network_token(context);
            msg->tokenLookup1 = NULL;
        }
        if (!ADDRESS_IS_NETWORK_TOKEN(context->contract_address_received) ||
            context->contract_received_unknown) {
            // Address is not network token (0xeee...) so we will need to look up the token in the
            // CAL.
            printf_hex_array("Setting address received to: ",
                             ADDRESS_LENGTH,
                             context->contract_address_received);
            msg->tokenLookup2 = context->contract_address_received;
        } else {
            received_network_token(context);
            msg->tokenLookup2 = NULL;
        }

        msg->uiType = ETH_UI_TYPE_GENERIC;
        msg->result = ETH_PLUGIN_RESULT_OK;
    } else {
        PRINTF("Context not valid\n");
        msg->result = ETH_PLUGIN_RESULT_FALLBACK;
    }
}
