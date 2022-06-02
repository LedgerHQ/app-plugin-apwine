#include "apwine_plugin.h"

// Need more information about the interface for plugins? Please read the README.md!

// swapExactAmountIn 0xe7ffb5f7
static const uint8_t APWINE_SWAP_EXACT_AMOUNT_IN_SELECTOR[SELECTOR_SIZE] = {0xe7, 0xff, 0xb5, 0xf7};

// swapExactAmountOut 0x16dc3ace
static const uint8_t APWINE_SWAP_EXACT_AMOUNT_OUT_SELECTOR[SELECTOR_SIZE] = {0x16,
                                                                             0xdc,
                                                                             0x3a,
                                                                             0xce};

// addLiquity 0x2d8918d7
static const uint8_t APWINE_ADD_LIQUIDITY_SELECTOR[SELECTOR_SIZE] = {0x2d, 0x89, 0x18, 0xd7};

// removeLiquity 0xafc3083c
static const uint8_t APWINE_REMOVE_LIQUIDITY_SELECTOR[SELECTOR_SIZE] = {0xaf, 0xc3, 0x08, 0x3c};

// deposit 0x47e7ef24
static const uint8_t APWINE_DEPOSIT_SELECTOR[SELECTOR_SIZE] = {0x47, 0xe7, 0xef, 0x24};

// withdraw 0xf3fef3a3
static const uint8_t APWINE_WITHDRAW_SELECTOR[SELECTOR_SIZE] = {0xf3, 0xfe, 0xf3, 0xa3};

// zapInToPT 0xff3bf066
static const uint8_t APWINE_ZAPINTOPT_SELECTOR[SELECTOR_SIZE] = {0xff, 0x3b, 0xf0, 0x66};

// redeemYield 0x258d3c33
static const uint8_t APWINE_REDEEM_YIELD_SELECTOR[SELECTOR_SIZE] = {0x25, 0x8d, 0x3c, 0x33};

// increase_amount 0x4957677c
static const uint8_t APWINE_INCREASE_AMOUNT_SELECTOR[SELECTOR_SIZE] = {0x49, 0x57, 0x67, 0x7c};

// create_lock 0x65fc3873
static const uint8_t APWINE_CREATE_LOCK_SELECTOR[SELECTOR_SIZE] = {0x65, 0xfc, 0x38, 0x73};

// increase_unlock_time 0xeff7a612
static const uint8_t APWINE_INCREASE_UNLOCK_TIME_SELECTOR[SELECTOR_SIZE] = {0xef, 0xf7, 0xa6, 0x12};

// Array of all the different apwine selectors.
const uint8_t *const APWINE_SELECTORS[NUM_APWINE_SELECTORS] = {
    APWINE_SWAP_EXACT_AMOUNT_IN_SELECTOR,
    APWINE_SWAP_EXACT_AMOUNT_OUT_SELECTOR,
    APWINE_ADD_LIQUIDITY_SELECTOR,
    APWINE_REMOVE_LIQUIDITY_SELECTOR,
    APWINE_DEPOSIT_SELECTOR,
    APWINE_WITHDRAW_SELECTOR,
    APWINE_ZAPINTOPT_SELECTOR,
    APWINE_REDEEM_YIELD_SELECTOR,
    APWINE_INCREASE_AMOUNT_SELECTOR,
    APWINE_CREATE_LOCK_SELECTOR,
    APWINE_INCREASE_UNLOCK_TIME_SELECTOR,
};

// apwine uses `0xeeeee` as a dummy address to represent ETH.
const uint8_t APWINE_ETH_ADDRESS[ADDRESS_LENGTH] = {0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
                                                    0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
                                                    0xee, 0xee, 0xee, 0xee, 0xee, 0xee};

// Used to indicate that the beneficiary should be the sender.
const uint8_t NULL_ETH_ADDRESS[ADDRESS_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Indicate which token is send and received
const uint8_t CONTRACT_ADDRESS_TOKEN_PATH[ADDRESS_LENGTH] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const contract_address_ticker_t CONTRACT_ADDRESS_COLLECTION[NUM_CONTRACT_ADDRESS_COLLECTION] = {
    {{0xc6, 0x1c, 0x0f, 0x49, 0x61, 0xf2, 0x09, 0x3a, 0x08, 0x3f,
      0x47, 0xa4, 0xb7, 0x83, 0xad, 0x26, 0x0d, 0xea, 0xf7, 0xea},
     "stkAAVE",
     "90D-Paladin-palStkAAVE-1",
     18},
    {{0x16, 0x04, 0xc5, 0xe9, 0xab, 0x48, 0x8d, 0x66, 0xe9, 0x83,
      0x64, 0x43, 0x55, 0x51, 0x1d, 0xce, 0xf5, 0xc3, 0x2e, 0xdf},
     "90D-Lido-stETH",
     "WETH",
     18},
    {{0xa4, 0x08, 0x5c, 0x10, 0x6c, 0x7a, 0x9a, 0x7a, 0xd0, 0x57,
      0x48, 0x65, 0xbb, 0xd7, 0xca, 0xc5, 0xe1, 0x09, 0x81, 0x95},
     "90D-ParaSwap-SPSP_PP4",
     "PSP",
     18},
    {{0x0C, 0xC3, 0x6e, 0x3c, 0xc5, 0xeA, 0xCA, 0x6d, 0x04, 0x6b,
      0x53, 0x77, 0x03, 0xae, 0x94, 0x68, 0x74, 0xd5, 0x72, 0x99},
     "90D-TrueFi-tfUSDC",
     "USDC",
     6},
    {{0x83, 0x9b, 0xb0, 0x33, 0x73, 0x85, 0x10, 0xaa, 0x6b, 0x4f,
      0x78, 0xaf, 0x20, 0xf0, 0x66, 0xbd, 0xc8, 0x24, 0xb1, 0x89},
     "90D-StakeDAO-sdFRAX3CRV-f",
     "FRAX3CRV-f",
     18},
    {{0xb9, 0x32, 0xc4, 0x80, 0x12, 0x40, 0x75, 0x36, 0x04, 0xc7,
      0x68, 0xc9, 0x91, 0xeb, 0x64, 0x0b, 0xcd, 0x7c, 0x06, 0xeb},
     "90D-Aave-aUSDT",
     "USDT",
     6},
    {{0x49, 0xcb, 0xbf, 0xed, 0xb1, 0x5b, 0x5c, 0x22, 0xca, 0xc5,
      0x3d, 0xaf, 0x10, 0x45, 0x12, 0xa5, 0xde, 0x9c, 0x84, 0x57},
     "90D-ParaSwap-SPSP_PP3",
     "PSP",
     18},
    {{0xcb, 0xa9, 0x60, 0x00, 0x13, 0x07, 0xa1, 0x6c, 0xe8, 0xa9,
      0xe3, 0x26, 0xd7, 0x3e, 0x92, 0xd5, 0x3b, 0x44, 0x6e, 0x81},
     "90D-Sushi-xSUSHI",
     "SUSHI",
     18},
    {{0xbc, 0x35, 0xb7, 0x0c, 0xcc, 0x8e, 0xf4, 0xec, 0x1c, 0xcc,
      0x34, 0xfa, 0xb6, 0x0c, 0xcb, 0xba, 0x16, 0x20, 0x11, 0xe4},
     "90D-Yearn-yvCurve-ibEUR",
     "ibEUR+sEUR-f",
     18},
    {{0x10, 0x89, 0xf7, 0xbb, 0xf8, 0xc6, 0x80, 0xdb, 0x92, 0x75,
      0x9a, 0x30, 0xd4, 0x2d, 0xdf, 0xba, 0x7c, 0x79, 0x4b, 0xd2},
     "90D-Olympus-gOHM",
     "OHM",
     18},
    {{0x4d, 0xf9, 0xbb, 0x88, 0x1e, 0x5e, 0x61, 0x03, 0x40, 0x01,
      0x44, 0x0a, 0xaa, 0xff, 0x2f, 0xb2, 0x93, 0x2e, 0x28, 0x83},
     "90D-Harvest-iFARM",
     "FARM",
     18},
    {{0x72, 0x59, 0x11, 0x4d, 0xf3, 0x63, 0xde, 0x5d, 0x42, 0xfd,
      0xf0, 0x0b, 0x70, 0x5f, 0xad, 0x7c, 0x85, 0xf8, 0xf7, 0x95},
     "90D-Yearn-yvCurve-3Crypto",
     "crv3crypto",
     18},
    {{0xea, 0x85, 0x15, 0x03, 0xff, 0x41, 0x6e, 0x34, 0x58, 0x5d,
      0x28, 0xc2, 0x48, 0x91, 0x83, 0x44, 0xc5, 0x69, 0xb2, 0x19},
     "90D-IDLE Finance-AA_idleCvxFRAX3CRV-f",
     "FRAX3CRV-f",
     18},
    {{0x1a, 0x65, 0x25, 0xe4, 0xa4, 0xab, 0x2e, 0x3a, 0xea, 0x7e,
      0xd3, 0xcf, 0x81, 0x3e, 0x8e, 0xd0, 0x7f, 0xa3, 0x44, 0x6d},
     "90D-Yearn-yvCurve-IronBank",
     "ib3CRV",
     18},
};