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

// Indicate the Ethereum chain ID
const uint8_t ETH_CHAIN_ID[ETH_CHAIN_ID_LENGTH] = {0x01};

const contract_address_ticker_t CONTRACT_ADDRESS_COLLECTION[NUM_CONTRACT_ADDRESS_COLLECTION] = {
    {{0xc6, 0x1c, 0x0f, 0x49, 0x61, 0xf2, 0x09, 0x3a, 0x08, 0x3f,
      0x47, 0xa4, 0xb7, 0x83, 0xad, 0x26, 0x0d, 0xea, 0xf7, 0xea},
     "90D-Paladin-palStkAAVE",    // PT
     "90D-Paladin-palStkAAVE-1",  // FYT
     "stkAAVE",                   // Underlying
     18},
    {{0xa4, 0x08, 0x5c, 0x10, 0x6c, 0x7a, 0x9a, 0x7a, 0xd0, 0x57,
      0x48, 0x65, 0xbb, 0xd7, 0xca, 0xc5, 0xe1, 0x09, 0x81, 0x95},
     "90D-ParaSwap-SPSP_PP4",    // PT
     "90D-ParaSwap-SPSP_PP4-1",  // FYT
     "PSP",                      // Underlying
     18},
    {{0x0C, 0xC3, 0x6e, 0x3c, 0xc5, 0xeA, 0xCA, 0x6d, 0x04, 0x6b,
      0x53, 0x77, 0x03, 0xae, 0x94, 0x68, 0x74, 0xd5, 0x72, 0x99},
     "90D-TrueFi-tfUSDC",    // PT
     "90D-TrueFi-tfUSDC-1",  // FYT
     "USDC",                 // Underlying
     6},
    {{0x83, 0x9b, 0xb0, 0x33, 0x73, 0x85, 0x10, 0xaa, 0x6b, 0x4f,
      0x78, 0xaf, 0x20, 0xf0, 0x66, 0xbd, 0xc8, 0x24, 0xb1, 0x89},
     "90D-StakeDAO-sdFRAX3CRV-f",    // PT
     "90D-StakeDAO-sdFRAX3CRV-f-1",  // FYT
     "FRAX3CRV-f",                   // Underlying
     18},
    {{0xb9, 0x32, 0xc4, 0x80, 0x12, 0x40, 0x75, 0x36, 0x04, 0xc7,
      0x68, 0xc9, 0x91, 0xeb, 0x64, 0x0b, 0xcd, 0x7c, 0x06, 0xeb},
     "90D-Aave-aUSDT",    // PT
     "90D-Aave-aUSDT-0",  // FYT
     "USDT",              // Underlying
     6},
    {{0x49, 0xcb, 0xbf, 0xed, 0xb1, 0x5b, 0x5c, 0x22, 0xca, 0xc5,
      0x3d, 0xaf, 0x10, 0x45, 0x12, 0xa5, 0xde, 0x9c, 0x84, 0x57},
     "90D-ParaSwap-SPSP_PP3",    // PT
     "90D-ParaSwap-SPSP_PP3-1",  // FYT
     "PSP",                      // Underlying
     18},
    {{0xcb, 0xa9, 0x60, 0x00, 0x13, 0x07, 0xa1, 0x6c, 0xe8, 0xa9,
      0xe3, 0x26, 0xd7, 0x3e, 0x92, 0xd5, 0x3b, 0x44, 0x6e, 0x81},
     "90D-Sushi-xSUSHI",    // PT
     "90D-Sushi-xSUSHI-1",  // FYT
     "SUSHI",               // Underlying
     18},
    {{0xbc, 0x35, 0xb7, 0x0c, 0xcc, 0x8e, 0xf4, 0xec, 0x1c, 0xcc,
      0x34, 0xfa, 0xb6, 0x0c, 0xcb, 0xba, 0x16, 0x20, 0x11, 0xe4},
     "90D-Yearn-yvCurve-ibEUR",    // PT
     "90D-Yearn-yvCurve-ibEUR-0",  // FYT
     "ibEUR+sEUR-f",               // Underlying
     18},
    {{0x10, 0x89, 0xf7, 0xbb, 0xf8, 0xc6, 0x80, 0xdb, 0x92, 0x75,
      0x9a, 0x30, 0xd4, 0x2d, 0xdf, 0xba, 0x7c, 0x79, 0x4b, 0xd2},
     "90D-Olympus-gOHM",    // PT
     "90D-Olympus-gOHM-0",  // FYT
     "OHM",                 // Underlying
     18},
    {{0x4d, 0xf9, 0xbb, 0x88, 0x1e, 0x5e, 0x61, 0x03, 0x40, 0x01,
      0x44, 0x0a, 0xaa, 0xff, 0x2f, 0xb2, 0x93, 0x2e, 0x28, 0x83},
     "90D-Harvest-iFARM",    // PT
     "90D-Harvest-iFARM-0",  // FYT
     "FARM",                 // Underlying
     18},
    {{0x72, 0x59, 0x11, 0x4d, 0xf3, 0x63, 0xde, 0x5d, 0x42, 0xfd,
      0xf0, 0x0b, 0x70, 0x5f, 0xad, 0x7c, 0x85, 0xf8, 0xf7, 0x95},
     "90D-Yearn-yvCurve-3Crypto",    // PT
     "90D-Yearn-yvCurve-3Crypto-0",  // FYT
     "crv3crypto",                   // Underlying
     18},
    {{0xea, 0x85, 0x15, 0x03, 0xff, 0x41, 0x6e, 0x34, 0x58, 0x5d,
      0x28, 0xc2, 0x48, 0x91, 0x83, 0x44, 0xc5, 0x69, 0xb2, 0x19},
     "90D-IDLE Finance-AA_idleCvxFRAX3CRV-f",    // PT
     "90D-IDLE Finance-AA_idleCvxFRAX3CRV-f-0",  // FYT
     "FRAX3CRV-f",                               // Underlying
     18},
    {{0x1a, 0x65, 0x25, 0xe4, 0xa4, 0xab, 0x2e, 0x3a, 0xea, 0x7e,
      0xd3, 0xcf, 0x81, 0x3e, 0x8e, 0xd0, 0x7f, 0xa3, 0x44, 0x6d},
     "90D-Yearn-yvCurve-IronBank",    // PT
     "90D-Yearn-yvCurve-IronBank-0",  // FYT
     "ib3CRV",                        // Underlying
     18},
    {{0x8a, 0x36, 0x2a, 0xa1, 0xc8, 0x1e, 0xd0, 0xee, 0x2a, 0xe6,
      0x77, 0xa8, 0xb5, 0x9e, 0x0f, 0x56, 0x3d, 0xd2, 0x90, 0xba},
     "90D-Aave-amUSDC",    // PT
     "90D-Aave-amUSDC-1",  // FYT
     "USD Coin",           // Underlying
     6},
    {{0x91, 0xe9, 0x4e, 0x5e, 0x3b, 0xaa, 0x05, 0x4f, 0x92, 0xba,
      0xc4, 0x8a, 0x9c, 0x05, 0xe6, 0x22, 0x8d, 0xe1, 0xfc, 0xac},
     "90D-Beefy-mooJarvis4eur",    // PT
     "90D-Beefy-mooJarvis4eur-1",  // FYT
     "4eur-f",                     // Underlying
     18},
    {{0xc6, 0x8b, 0x69, 0x87, 0x07, 0x59, 0x44, 0xf9, 0xe8, 0xb0,
      0xa6, 0xc2, 0xb5, 0x2e, 0x92, 0x3b, 0xc1, 0xfb, 0x90, 0x28},
     "90D-StakeDAO-sdam3CRV",    // PT
     "90D-StakeDAO-sdam3CRV-1",  // FYT
     "am3CRV",                   // Underlying
     18},
    {{0x2d, 0xf9, 0xcf, 0x57, 0xac, 0x79, 0xa7, 0x6a, 0x2e, 0x8d,
      0x94, 0x4b, 0x62, 0x3a, 0x9f, 0x32, 0x4a, 0xf8, 0x5a, 0xb8},
     "90D-Harvest-bfBP-BTC-SP",    // PT
     "90D-Harvest-bfBP-BTC-SP-0",  // FYT
     "BP-BTC-SP",                  // Underlying
     18},
    {{0x74, 0x29, 0xe1, 0x60, 0xaa, 0x4a, 0xb7, 0xbb, 0xec, 0x65,
      0xc1, 0x01, 0xbd, 0x26, 0x24, 0xc8, 0xcb, 0xa8, 0xa2, 0xf6},
     "90D-Beefy-mooJarvis2jpy",    // PT
     "90D-Beefy-mooJarvis2jpy-1",  // FYT
     "2jpy-f",                     // Underlying
     18},
    {{0x4b, 0xf9, 0x82, 0xf4, 0x39, 0x94, 0x03, 0x3e, 0x8c, 0x4e,
      0x2c, 0x36, 0xd0, 0x0c, 0x77, 0x6b, 0x55, 0x0e, 0x76, 0xc3},
     "90D-Beefy-mooMaiUSDC-miMATIC",    // PT
     "90D-Beefy-mooMaiUSDC-miMATIC-0",  // FYT
     "Uniswap V2",                      // Underlying
     18},
};

const contract_address_future_vault_t
    CONTRACT_ADDRESS_FUTURE_VAULT[NUM_CONTRACT_ADDRESS_FUTURE_VAULT] = {
        {{0x89, 0x4d, 0x7e, 0x0f, 0x2e, 0xcf, 0xf8, 0x66, 0x27, 0x5a,
          0x5a, 0x09, 0xec, 0x6d, 0x44, 0x71, 0x4f, 0xc7, 0x4c, 0x35},
         "Governance OHM",
         "90D-Olympus-gOHM",
         "N/A",
         "N/A",
         18},
        {{0x35, 0xbb, 0xdc, 0x3f, 0xbd, 0xc2, 0x6f, 0x7d, 0xfe, 0xe5,
          0x67, 0x0a, 0xf5, 0x0b, 0x93, 0xc7, 0xea, 0xbc, 0xe2, 0xc0},
         "stETH",
         "90D-Lido-stETH",
         "mooCurveATriCrypto3",
         "90D-Beefy-mooCurveATriCrypto3",
         18},
        {{0xb9, 0xdf, 0x66, 0x0c, 0xaa, 0xa6, 0x2d, 0x47, 0xdf, 0x26,
          0x5a, 0x46, 0x9c, 0x8b, 0x77, 0xf6, 0x61, 0xef, 0xc1, 0x8d},
         "SPSP_PP4",
         "90D-ParaSwap-SPSP_PP4",
         "N/A",
         "N/A",
         18},
        {{0x8e, 0x6c, 0xa2, 0xb6, 0x3b, 0x0c, 0x23, 0x13, 0x64, 0xf8,
          0x5c, 0x42, 0xbc, 0xfc, 0x9d, 0x0a, 0x49, 0x78, 0x6e, 0x62},
         "SPSP_PP3",
         "90D-ParaSwap-SPSP_PP3",
         "N/A",
         "N/A",
         18},
        {{0xa0, 0xed, 0x6d, 0xad, 0x32, 0x19, 0x44, 0x22, 0x24, 0xd8,
          0x6f, 0xae, 0xc5, 0x32, 0xf8, 0x90, 0xca, 0xbf, 0x14, 0x83},
         "sdFRAX3CRV-f",
         "90D-StakeDAO-sdFRAX3CRV-f",
         "N/A",
         "N/A",
         18},
        {{0x1d, 0x22, 0x0b, 0xd7, 0x6c, 0x2f, 0x00, 0x4b, 0xde, 0x29,
          0x58, 0xc6, 0x43, 0x17, 0x31, 0x89, 0xd8, 0xdb, 0x3a, 0x7a},
         "yvCurve-ibEUR",
         "90D-Yearn-yvCurve-ibEUR",
         "N/A",
         "N/A",
         18},
        {{0x73, 0x48, 0xf3, 0xbd, 0x47, 0x47, 0xff, 0xe1, 0x42, 0xed,
          0x54, 0x10, 0x59, 0x62, 0xe3, 0x61, 0xdd, 0xd0, 0xdb, 0x11},
         "iFARM",
         "90D-Harvest-iFARM",
         "N/A",
         "N/A",
         18},
        {{0x19, 0x48, 0x1d, 0x0b, 0x31, 0x77, 0xae, 0xde, 0xc7, 0x0b,
          0x83, 0x39, 0xf0, 0x70, 0x6a, 0x79, 0xb9, 0x84, 0x5b, 0xe7},
         "xSUSHI",
         "90D-Sushi-xSUSHI",
         "N/A",
         "N/A",
         18},
        {{0xbd, 0x0b, 0xa0, 0x83, 0xac, 0xa4, 0x8b, 0xf6, 0xbe, 0x03,
          0x48, 0x90, 0x00, 0x6e, 0x92, 0xb8, 0x74, 0x78, 0x33, 0x65},
         "palStkAAVE",
         "90D-Paladin-palStkAAVE",
         "bf4eur-f",
         "90D-Harvest-bf4eur-f",
         18},
        {{0x6f, 0xb5, 0x66, 0xcb, 0x80, 0xa5, 0x03, 0x8b, 0xbe, 0x04,
          0x21, 0xa9, 0x1d, 0x9f, 0x96, 0xf9, 0xbb, 0x9d, 0x6d, 0x95},
         "tfUSDC",
         "90D-TrueFi-tfUSDC",
         "N/A",
         "N/A",
         6},
        {{0x26, 0x1c, 0xa4, 0xe6, 0x45, 0x57, 0x8a, 0x9e, 0xd3, 0x04,
          0xac, 0x98, 0xd5, 0x24, 0x3e, 0xbb, 0x51, 0x8a, 0x16, 0x2d},
         "yvCurve-3Crypto",
         "90D-Yearn-yvCurve-3Crypto",
         "N/A",
         "N/A",
         18},
        {{0x0f, 0xd1, 0x3d, 0xca, 0x30, 0xee, 0x1b, 0x98, 0x8d, 0xbd,
          0xf1, 0x37, 0x4c, 0x15, 0x61, 0x14, 0x13, 0x85, 0xbe, 0xe6},
         "AA_idleCvxFRAX3CRV-f",
         "90D-IDLE Finance-AA_idleCvxFRAX3CRV-f",
         "N/A",
         "N/A",
         18},
        {{0x60, 0x9e, 0xbd, 0x0a, 0x8b, 0x06, 0xda, 0xbb, 0x80, 0x5f,
          0x1e, 0x64, 0xb3, 0x53, 0x01, 0xc1, 0x85, 0xd9, 0x4f, 0x95},
         "yvCurve-IronBank",
         "90D-Yearn-yvCurve-IronBank",
         "N/A",
         "N/A",
         18},
        {{0x10, 0xf2, 0x9c, 0xf8, 0xe6, 0xba, 0x81, 0x36, 0x3a, 0x33,
          0x0c, 0x50, 0xf7, 0x49, 0x00, 0xd0, 0x3c, 0xd6, 0x32, 0x4d},
         "xSDT",
         "90D-StakeDAO-xSDT",
         "amUSDC",
         "90D-Aave-amUSDC",
         18},
        {{0xb5, 0x24, 0xc1, 0x63, 0x30, 0xa7, 0x61, 0x82, 0xef, 0x61,
          0x7f, 0x08, 0xf5, 0xe6, 0x99, 0x6f, 0x57, 0x7a, 0xc6, 0x4a},
         "aUSDT",
         "90D-Aave-aUSDT",
         "N/A",
         "N/A",
         6},
        {{0x8a, 0x28, 0x3d, 0x41, 0x1b, 0xbe, 0x43, 0x25, 0x9e, 0xf8,
          0xf3, 0x5c, 0xf5, 0xef, 0xf1, 0xa4, 0x87, 0xf6, 0x0c, 0xf9},
         "N/A",
         "N/A",
         "bfBP-BTC-SP",
         "90D-Harvest-bfBP-BTC-SP",
         18},
        {{0x73, 0xa6, 0x8d, 0x50, 0x7d, 0xae, 0x80, 0x05, 0x42, 0xa0,
          0xd1, 0x2b, 0x56, 0xd2, 0xdd, 0x16, 0x96, 0x49, 0xa9, 0x1c},
         "N/A",
         "N/A",
         "mooMaiUSDC-miMATIC",
         "90D-Beefy-mooMaiUSDC-miMATIC",
         18},
        {{0x4d, 0x17, 0x6f, 0xeb, 0x1f, 0xe8, 0xa4, 0x9d, 0x52, 0x79,
          0x93, 0x97, 0xf0, 0xb1, 0x61, 0xfb, 0x15, 0xb2, 0x3a, 0x50},
         "N/A",
         "N/A",
         "mooJarvis4eur",
         "90D-Beefy-mooJarvis4eur",
         18},
        {{0x4c, 0xce, 0x82, 0xd6, 0x55, 0xcb, 0x27, 0xc9, 0x8a, 0x3e,
          0x5e, 0x6b, 0xc9, 0xc7, 0x5b, 0x4a, 0xc2, 0x57, 0xf5, 0x40},
         "N/A",
         "N/A",
         "mooJarvis2jpy",
         "90D-Beefy-mooJarvis2jpy",
         18},
        {{0xb8, 0x5c, 0xd2, 0x93, 0x27, 0x95, 0x1e, 0xdd, 0x9f, 0xfd,
          0x6a, 0x8c, 0xe5, 0xfa, 0x55, 0x84, 0x18, 0x38, 0xd0, 0x2a},
         "N/A",
         "N/A",
         "sdam3CRV",
         "90D-StakeDAO-sdam3CRV",
         18},
};

const contract_address_ticker_polygon_eth_t
    CONTRACT_ADDRESS_COLLECTION_2[NUM_CONTRACT_ADDRESS_COLLECTION_2] = {
        {{0x16, 0x04, 0xc5, 0xe9, 0xab, 0x48, 0x8d, 0x66, 0xe9, 0x83,
          0x64, 0x43, 0x55, 0x51, 0x1d, 0xce, 0xf5, 0xc3, 0x2e, 0xdf},
         "90D-Lido-stETH",                   // Ethereum PT
         "90D-Lido-stETH-1",                 // Ethereum FYT
         "WETH",                             // Ethereum Underlying
         "90D-Beefy-mooCurveATriCrypto3",    // Polygon PT
         "90D-Beefy-mooCurveATriCrypto3-1",  // Polygon FYT
         "Curve USD-BTC-ETH",                // Polygon Underlying
         18},
};
