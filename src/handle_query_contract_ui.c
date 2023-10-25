#include <stdbool.h>
#include "apwine_plugin.h"

// Set UI for retrieving the ticker.
// The ticker is calculated based on the pair_path_first and the token_path_sent.
// The pair 0 == (PT,Underlying) and 1 == (PT, FYT)
// If the token path is 0, the ticker is the PT ticker, and if the token path is 1,
// the ticker is the Underlying or FYT ticker.
static bool set_send_ticker_ui(ethQueryContractUI_t *msg, apwine_parameters_t *context) {
    switch (context->selectorIndex) {
        case SWAP_EXACT_AMOUNT_IN:
        case SWAP_EXACT_AMOUNT_OUT:
            strlcpy(msg->title, "Token Send", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            return false;
    }

    // set to a default value like “???” so that is displayed something is wrong to the user.
    strlcpy(msg->msg, "???", msg->msgLength);

    contract_address_ticker_polygon_eth_t *currentToken2 =
        (contract_address_ticker_polygon_eth_t *) PIC(&CONTRACT_ADDRESS_COLLECTION_2[0]);
    if (memcmp(currentToken2->_amm, context->contract_address_sent, ADDRESS_LENGTH) == 0) {
        if (memcmp(msg->pluginSharedRO->txContent->chainID.value,
                   ETH_CHAIN_ID,
                   ETH_CHAIN_ID_LENGTH) == 0) {
            if (context->pair_path_first == 0) {
                if (context->token_path_sent == 0) {
                    strlcpy(msg->msg, currentToken2->ticker_eth_pt, msg->msgLength);
                } else if (context->token_path_sent == 1) {
                    strlcpy(msg->msg, currentToken2->ticker_eth_underlying, msg->msgLength);
                }
            } else {
                if (context->token_path_sent == 0) {
                    strlcpy(msg->msg, currentToken2->ticker_eth_pt, msg->msgLength);
                } else if (context->token_path_sent == 1) {
                    strlcpy(msg->msg, currentToken2->ticker_eth_fyt, msg->msgLength);
                }
            }
        } else {
            if (context->pair_path_first == 0) {
                if (context->token_path_sent == 0) {
                    strlcpy(msg->msg, currentToken2->ticker_polygon_pt, msg->msgLength);
                } else if (context->token_path_sent == 1) {
                    strlcpy(msg->msg, currentToken2->ticker_polygon_underlying, msg->msgLength);
                }
            } else {
                if (context->token_path_sent == 0) {
                    strlcpy(msg->msg, currentToken2->ticker_polygon_pt, msg->msgLength);
                } else if (context->token_path_sent == 1) {
                    strlcpy(msg->msg, currentToken2->ticker_polygon_fyt, msg->msgLength);
                }
            }
        }
    }

    contract_address_ticker_t *currentToken = NULL;
    for (uint8_t i = 0; i < NUM_CONTRACT_ADDRESS_COLLECTION; i++) {
        currentToken = (contract_address_ticker_t *) PIC(&CONTRACT_ADDRESS_COLLECTION[i]);
        if (memcmp(currentToken->_amm, context->contract_address_sent, ADDRESS_LENGTH) == 0) {
            if (context->pair_path_first == 0) {
                if (context->token_path_sent == 0) {
                    strlcpy(msg->msg, currentToken->ticker_pt, msg->msgLength);
                } else if (context->token_path_sent == 1) {
                    strlcpy(msg->msg, currentToken->ticker_underlying, msg->msgLength);
                }
            } else {
                if (context->token_path_sent == 0) {
                    strlcpy(msg->msg, currentToken->ticker_pt, msg->msgLength);
                } else if (context->token_path_sent == 1) {
                    strlcpy(msg->msg, currentToken->ticker_fyt, msg->msgLength);
                }
            }
        }
    }
    return true;
}

// Set UI for retrieving the ticker.
// The ticker is calculated based on the pair_path_first,pair_path_last and the token_path_received.
// The pair 0 == (PT,Underlying) and 1 == (PT, FYT)
// If the pair_path array length is greater than 1, it means that there are multiple swaps.
// Therefore we will use the last pair_path to determine the ticker.
// Otherwise, we will use the first pair_path.
// If the token path is 0, the ticker is the PT ticker, and if the token path is 1,
// the ticker is the Underlying or FYT ticker.
static bool set_receive_ticker_ui(ethQueryContractUI_t *msg, apwine_parameters_t *context) {
    switch (context->selectorIndex) {
        case SWAP_EXACT_AMOUNT_IN:
        case SWAP_EXACT_AMOUNT_OUT:
            strlcpy(msg->title, "Token Receive", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", &context->selectorIndex);
            return false;
    }

    // set to a default value like “???” so that is displayed something is wrong to the user.
    strlcpy(msg->msg, "???", msg->msgLength);

    contract_address_ticker_polygon_eth_t *currentToken2 =
        (contract_address_ticker_polygon_eth_t *) PIC(&CONTRACT_ADDRESS_COLLECTION_2[0]);
    if (memcmp(currentToken2->_amm, context->contract_address_sent, ADDRESS_LENGTH) == 0) {
        if (memcmp(msg->pluginSharedRO->txContent->chainID.value,
                   ETH_CHAIN_ID,
                   ETH_CHAIN_ID_LENGTH) == 0) {
            if (context->array_len <= 1) {
                if (context->pair_path_first == 0) {
                    if (context->token_path_received == 0) {
                        strlcpy(msg->msg, currentToken2->ticker_eth_pt, msg->msgLength);
                    } else if (context->token_path_received == 1) {
                        strlcpy(msg->msg, currentToken2->ticker_eth_underlying, msg->msgLength);
                    }
                } else {
                    if (context->token_path_received == 0) {
                        strlcpy(msg->msg, currentToken2->ticker_eth_pt, msg->msgLength);
                    } else if (context->token_path_received == 1) {
                        strlcpy(msg->msg, currentToken2->ticker_eth_fyt, msg->msgLength);
                    }
                }
            } else {
                if (context->pair_path_last == 0) {
                    if (context->token_path_received == 0) {
                        strlcpy(msg->msg, currentToken2->ticker_eth_pt, msg->msgLength);
                    } else if (context->token_path_received == 1) {
                        strlcpy(msg->msg, currentToken2->ticker_eth_underlying, msg->msgLength);
                    }
                } else {
                    if (context->token_path_received == 0) {
                        strlcpy(msg->msg, currentToken2->ticker_eth_pt, msg->msgLength);
                    } else if (context->token_path_received == 1) {
                        strlcpy(msg->msg, currentToken2->ticker_eth_fyt, msg->msgLength);
                    }
                }
            }
        } else {
            if (context->array_len <= 1) {
                if (context->pair_path_first == 0) {
                    if (context->token_path_received == 0) {
                        strlcpy(msg->msg, currentToken2->ticker_polygon_pt, msg->msgLength);
                    } else if (context->token_path_received == 1) {
                        strlcpy(msg->msg, currentToken2->ticker_polygon_underlying, msg->msgLength);
                    }
                } else {
                    if (context->token_path_received == 0) {
                        strlcpy(msg->msg, currentToken2->ticker_polygon_pt, msg->msgLength);
                    } else if (context->token_path_received == 1) {
                        strlcpy(msg->msg, currentToken2->ticker_polygon_fyt, msg->msgLength);
                    }
                }
            } else {
                if (context->pair_path_last == 0) {
                    if (context->token_path_received == 0) {
                        strlcpy(msg->msg, currentToken2->ticker_polygon_pt, msg->msgLength);
                    } else if (context->token_path_received == 1) {
                        strlcpy(msg->msg, currentToken2->ticker_polygon_underlying, msg->msgLength);
                    }
                } else {
                    if (context->token_path_received == 0) {
                        strlcpy(msg->msg, currentToken2->ticker_polygon_pt, msg->msgLength);
                    } else if (context->token_path_received == 1) {
                        strlcpy(msg->msg, currentToken2->ticker_polygon_fyt, msg->msgLength);
                    }
                }
            }
        }
    }

    contract_address_ticker_t *currentToken = NULL;
    for (uint8_t i = 0; i < NUM_CONTRACT_ADDRESS_COLLECTION; i++) {
        currentToken = (contract_address_ticker_t *) PIC(&CONTRACT_ADDRESS_COLLECTION[i]);
        if (memcmp(currentToken->_amm, context->contract_address_sent, ADDRESS_LENGTH) == 0) {
            if (context->array_len <= 1) {
                if (context->pair_path_first == 0) {
                    if (context->token_path_received == 0) {
                        strlcpy(msg->msg, currentToken->ticker_pt, msg->msgLength);
                    } else if (context->token_path_received == 1) {
                        strlcpy(msg->msg, currentToken->ticker_underlying, msg->msgLength);
                    }
                } else {
                    if (context->token_path_received == 0) {
                        strlcpy(msg->msg, currentToken->ticker_pt, msg->msgLength);
                    } else if (context->token_path_received == 1) {
                        strlcpy(msg->msg, currentToken->ticker_fyt, msg->msgLength);
                    }
                }
            } else {
                if (context->pair_path_last == 0) {
                    if (context->token_path_received == 0) {
                        strlcpy(msg->msg, currentToken->ticker_pt, msg->msgLength);
                    } else if (context->token_path_received == 1) {
                        strlcpy(msg->msg, currentToken->ticker_underlying, msg->msgLength);
                    }
                } else {
                    if (context->token_path_received == 0) {
                        strlcpy(msg->msg, currentToken->ticker_pt, msg->msgLength);
                    } else if (context->token_path_received == 1) {
                        strlcpy(msg->msg, currentToken->ticker_fyt, msg->msgLength);
                    }
                }
            }
        }
    }
    return true;
}

// Set UI for retrieving the underlying ticker.
static bool set_send_underlying_ui(ethQueryContractUI_t *msg, apwine_parameters_t *context) {
    switch (context->selectorIndex) {
        case ZAPINTOPT:
            strlcpy(msg->title, "Token Send", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            return false;
    }

    // set to a default value like “???” so that is displayed something is wrong to the user.
    strlcpy(msg->msg, "???", msg->msgLength);

    contract_address_ticker_polygon_eth_t *currentToken2 =
        (contract_address_ticker_polygon_eth_t *) PIC(&CONTRACT_ADDRESS_COLLECTION_2[0]);
    if (memcmp(currentToken2->_amm, context->contract_address_sent, ADDRESS_LENGTH) == 0) {
        if (memcmp(msg->pluginSharedRO->txContent->chainID.value,
                   ETH_CHAIN_ID,
                   ETH_CHAIN_ID_LENGTH) == 0) {
            strlcpy(msg->msg, currentToken2->ticker_eth_underlying, msg->msgLength);
        } else {
            strlcpy(msg->msg, currentToken2->ticker_polygon_underlying, msg->msgLength);
        }
    }

    contract_address_ticker_t *currentToken = NULL;
    for (uint8_t i = 0; i < NUM_CONTRACT_ADDRESS_COLLECTION; i++) {
        currentToken = (contract_address_ticker_t *) PIC(&CONTRACT_ADDRESS_COLLECTION[i]);
        if (memcmp(currentToken->_amm, context->contract_address_sent, ADDRESS_LENGTH) == 0) {
            strlcpy(msg->msg, currentToken->ticker_underlying, msg->msgLength);
        }
    }
    return true;
}

// Set UI for retrieving the PT ticker.
static bool set_receive_pt_ui(ethQueryContractUI_t *msg, apwine_parameters_t *context) {
    switch (context->selectorIndex) {
        case ZAPINTOPT:
            strlcpy(msg->title, "Token Receive", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", &context->selectorIndex);
            return false;
    }

    // set to a default value like “???” so that is displayed something is wrong to the user.
    strlcpy(msg->msg, "???", msg->msgLength);

    contract_address_ticker_polygon_eth_t *currentToken2 =
        (contract_address_ticker_polygon_eth_t *) PIC(&CONTRACT_ADDRESS_COLLECTION_2[0]);
    if (memcmp(currentToken2->_amm, context->contract_address_sent, ADDRESS_LENGTH) == 0) {
        if (memcmp(msg->pluginSharedRO->txContent->chainID.value,
                   ETH_CHAIN_ID,
                   ETH_CHAIN_ID_LENGTH) == 0) {
            strlcpy(msg->msg, currentToken2->ticker_eth_pt, msg->msgLength);
        } else {
            strlcpy(msg->msg, currentToken2->ticker_polygon_pt, msg->msgLength);
        }
    }

    contract_address_ticker_t *currentToken = NULL;
    for (uint8_t i = 0; i < NUM_CONTRACT_ADDRESS_COLLECTION; i++) {
        currentToken = (contract_address_ticker_t *) PIC(&CONTRACT_ADDRESS_COLLECTION[i]);
        if (memcmp(currentToken->_amm, context->contract_address_sent, ADDRESS_LENGTH) == 0) {
            strlcpy(msg->msg, currentToken->ticker_pt, msg->msgLength);
        }
    }
    return true;
}

// Set UI for retrieving the ticker used in deposit function.
static bool set_future_vault_token_deposit_ui(ethQueryContractUI_t *msg,
                                              apwine_parameters_t *context) {
    switch (context->selectorIndex) {
        case DEPOSIT:
            strlcpy(msg->title, "Token Send", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            return false;
    }

    // set to a default value like “???” so that is displayed something is wrong to the user.
    strlcpy(msg->msg, "???", msg->msgLength);

    contract_address_future_vault_t *currentToken = NULL;
    for (uint8_t i = 0; i < NUM_CONTRACT_ADDRESS_FUTURE_VAULT; i++) {
        currentToken = (contract_address_future_vault_t *) PIC(&CONTRACT_ADDRESS_FUTURE_VAULT[i]);
        if (memcmp(msg->pluginSharedRO->txContent->chainID.value,
                   ETH_CHAIN_ID,
                   ETH_CHAIN_ID_LENGTH) == 0) {
            if (memcmp(currentToken->_amm, context->contract_address_sent, ADDRESS_LENGTH) == 0) {
                strlcpy(msg->msg, currentToken->deposit_ticker_eth, msg->msgLength);
            }
        } else {
            if (memcmp(currentToken->_amm, context->contract_address_sent, ADDRESS_LENGTH) == 0) {
                strlcpy(msg->msg, currentToken->deposit_ticker_polygon, msg->msgLength);
            }
        }
    }
    return true;
}

// Set UI for retrieving the ticker used in withdraw function.
static bool set_future_vault_token_withdraw_ui(ethQueryContractUI_t *msg,
                                               apwine_parameters_t *context) {
    switch (context->selectorIndex) {
        case WITHDRAW:
            strlcpy(msg->title, "Token Receive", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return false;
    }

    // set to a default value like “???” so that is displayed something is wrong to the user.
    strlcpy(msg->msg, "???", msg->msgLength);

    contract_address_future_vault_t *currentToken = NULL;
    for (uint8_t i = 0; i < NUM_CONTRACT_ADDRESS_FUTURE_VAULT; i++) {
        currentToken = (contract_address_future_vault_t *) PIC(&CONTRACT_ADDRESS_FUTURE_VAULT[i]);
        if (memcmp(msg->pluginSharedRO->txContent->chainID.value,
                   ETH_CHAIN_ID,
                   ETH_CHAIN_ID_LENGTH) == 0) {
            if (memcmp(currentToken->_amm, context->contract_address_sent, ADDRESS_LENGTH) == 0) {
                strlcpy(msg->msg, currentToken->withdraw_ticker_eth, msg->msgLength);
            }
        } else {
            if (memcmp(currentToken->_amm, context->contract_address_sent, ADDRESS_LENGTH) == 0) {
                strlcpy(msg->msg, currentToken->withdraw_ticker_polygon, msg->msgLength);
            }
        }
    }
    return true;
}

// Set UI for "Send" screen.
static bool set_send_amount_ui(ethQueryContractUI_t *msg, apwine_parameters_t *context) {
    switch (context->selectorIndex) {
        case SWAP_EXACT_AMOUNT_IN:
        case SWAP_EXACT_AMOUNT_OUT:
            strlcpy(msg->title, "Amount In", msg->titleLength);
            break;
        case ADD_LIQUIDITY:
            strlcpy(msg->title, "First Amount In", msg->titleLength);
            break;
        case REMOVE_LIQUIDITY:
            strlcpy(msg->title, "First Amount Out", msg->titleLength);
            break;
        case DEPOSIT:
        case WITHDRAW:
        case INCREASE_AMOUNT:
        case CREATE_LOCK:
            strlcpy(msg->title, "Amount", msg->titleLength);
            break;
        case ZAPINTOPT:
            strlcpy(msg->title, "Send", msg->titleLength);
            break;
        case INCREASE_UNLOCK_TIME:
            strlcpy(msg->title, "Unlock time", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            return false;
    }

    // Convert to string.
    return amountToString(context->amount_sent,
                          INT256_LENGTH,
                          context->decimals_sent,
                          "",
                          msg->msg,
                          msg->msgLength);
}

// Set UI for "Send" screen.
static bool set_amount_ui(ethQueryContractUI_t *msg, apwine_parameters_t *context) {
    strlcpy(msg->title, "Amount", msg->titleLength);
    // Convert to string.
    return amountToString(msg->pluginSharedRO->txContent->value.value,
                          msg->pluginSharedRO->txContent->value.length,
                          WEI_TO_ETHER,
                          context->ticker_sent,
                          msg->msg,
                          msg->msgLength);
}

// Set UI for "Receive" screen.
static bool set_receive_amount_ui(ethQueryContractUI_t *msg, apwine_parameters_t *context) {
    switch (context->selectorIndex) {
        case SWAP_EXACT_AMOUNT_IN:
        case SWAP_EXACT_AMOUNT_OUT:
            strlcpy(msg->title, "Amount Out", msg->titleLength);
            break;
        case ADD_LIQUIDITY:
            strlcpy(msg->title, "2nd Amount In", msg->titleLength);
            break;
        case REMOVE_LIQUIDITY:
            strlcpy(msg->title, "2nd Amount Out", msg->titleLength);
            break;
        case ZAPINTOPT:
            strlcpy(msg->title, "Receive Min", msg->titleLength);
            break;
        case CREATE_LOCK:
            strlcpy(msg->title, "Unlock time", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            return false;
    }

    // Convert to string.
    return amountToString(context->amount_received,
                          INT256_LENGTH,
                          context->decimals_received,
                          "",
                          msg->msg,
                          msg->msgLength);
}

// Set UI for "Warning" screen.
static bool set_warning_ui(ethQueryContractUI_t *msg,
                           const apwine_parameters_t *context __attribute__((unused))) {
    strlcpy(msg->title, "WARNING", msg->titleLength);
    strlcpy(msg->msg, "Unknown token", msg->msgLength);
    return true;
}

uint8_t swap_exact_amount_screen(uint8_t index) {
    switch (index) {
        case 0:
            return SEND_TICKER_SCREEN;
        case 1:
            return SEND_SCREEN;
        case 2:
            return RECEIVE_TICKER_SCREEN;
        case 3:
            return RECEIVE_SCREEN;
        default:
            return ERROR;
    }
}

uint8_t swap_zapintopt_screen(uint8_t index) {
    switch (index) {
        case 0:
            return SEND_UNDERLYING_SCREEN;
        case 1:
            return SEND_SCREEN;
        case 2:
            return RECEIVE_PT_SCREEN;
        case 3:
            return RECEIVE_SCREEN;
        default:
            return ERROR;
    }
}

uint8_t swap_deposit_screen(uint8_t index) {
    switch (index) {
        case 0:
            return TOKEN_FUTURE_VAULT_SCREEN;
        case 1:
            return SEND_SCREEN;
        default:
            return ERROR;
    }
}

uint8_t swap_withdraw_screen(uint8_t index) {
    switch (index) {
        case 0:
            return WITHDRAW_FUTURE_VAULT_SCREEN;
        case 1:
            return SEND_SCREEN;
        default:
            return ERROR;
    }
}

uint8_t default_screen(uint8_t index, apwine_parameters_t *context) {
    bool token_sent_found = context->tokens_found & TOKEN_SENT_FOUND;
    bool token_received_found = context->tokens_found & TOKEN_RECEIVED_FOUND;

    bool both_tokens_found = token_received_found && token_sent_found;
    bool both_tokens_not_found = !token_received_found && !token_sent_found;

    switch (index) {
        case 0:
            if (both_tokens_found) {
                return SEND_SCREEN;
            } else if (both_tokens_not_found) {
                return WARN_SCREEN;
            } else if (token_sent_found) {
                return SEND_SCREEN;
            } else if (token_received_found) {
                return WARN_SCREEN;
            } else {
                return ERROR;
            }
        case 1:
            if (both_tokens_found) {
                return RECEIVE_SCREEN;
            } else if (both_tokens_not_found) {
                return SEND_SCREEN;
            } else if (token_sent_found) {
                return WARN_SCREEN;
            } else if (token_received_found) {
                return SEND_SCREEN;
            } else {
                return ERROR;
            }
        case 2:
            if (both_tokens_found) {
                return ERROR;
            } else if (both_tokens_not_found) {
                return WARN_SCREEN;
            } else {
                return RECEIVE_SCREEN;
            }
        case 3:
            if (both_tokens_found) {
                return ERROR;
            } else if (both_tokens_not_found) {
                return RECEIVE_SCREEN;
            }
            return ERROR;
        default:
            return ERROR;
    }
}

// Helper function that returns the enum corresponding to the screen that should be displayed.
static screens_t get_screen(ethQueryContractUI_t *msg, apwine_parameters_t *context) {
    uint8_t index = msg->screenIndex;
    switch (context->selectorIndex) {
        case REDEEM_YIELD:
            return AMOUNT_SCREEN;
        case SWAP_EXACT_AMOUNT_IN:
        case SWAP_EXACT_AMOUNT_OUT:
            return swap_exact_amount_screen(index);
        case ZAPINTOPT:
            return swap_zapintopt_screen(index);
        case DEPOSIT:
            return swap_deposit_screen(index);
        case WITHDRAW:
            return swap_withdraw_screen(index);
        default:
            return default_screen(index, context);
    }

    return ERROR;
}

void handle_query_contract_ui(ethQueryContractUI_t *msg) {
    apwine_parameters_t *context = (apwine_parameters_t *) msg->pluginContext;
    bool ret = false;

    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    screens_t screen = get_screen(msg, context);
    switch (screen) {
        case SEND_TICKER_SCREEN:
            ret = set_send_ticker_ui(msg, context);
            break;
        case SEND_UNDERLYING_SCREEN:
            ret = set_send_underlying_ui(msg, context);
            break;
        case TOKEN_FUTURE_VAULT_SCREEN:
            ret = set_future_vault_token_deposit_ui(msg, context);
            break;
        case WITHDRAW_FUTURE_VAULT_SCREEN:
            ret = set_future_vault_token_withdraw_ui(msg, context);
            break;
        case SEND_SCREEN:
            ret = set_send_amount_ui(msg, context);
            break;
        case AMOUNT_SCREEN:
            ret = set_amount_ui(msg, context);
            break;
        case RECEIVE_TICKER_SCREEN:
            ret = set_receive_ticker_ui(msg, context);
            break;
        case RECEIVE_PT_SCREEN:
            ret = set_receive_pt_ui(msg, context);
            break;
        case RECEIVE_SCREEN:
            ret = set_receive_amount_ui(msg, context);
            break;
        case WARN_SCREEN:
            ret = set_warning_ui(msg, context);
            break;
        default:
            PRINTF("Received an invalid screenIndex\n");
    }
    msg->result = ret ? ETH_PLUGIN_RESULT_OK : ETH_PLUGIN_RESULT_ERROR;
}
