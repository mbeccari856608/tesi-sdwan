/********************************************************
 * $Id: testbench.v 3220 2007-12-15 03:58:23Z jnaous $
 * 
 * Look at $NF2_ROOT/doc/register_map.ods for a detailed
 * description of the registers.
 * 
 * File automatically generated by testbench.v
 ********************************************************/
#ifndef _REG_DEFINES_
#define _REG_DEFINES_
#define TX_QUEUE_DISABLE_BIT_NUM                            0
#define RX_QUEUE_DISABLE_BIT_NUM                            1
#define RESET_MAC_BIT_NUM                                   2
#define MAC_DISABLE_TX_BIT_NUM                              3
#define MAC_DISABLE_RX_BIT_NUM                              4
#define MAC_DIS_JUMBO_TX_BIT_NUM                            5
#define MAC_DIS_JUMBO_RX_BIT_NUM                            6
#define MAC_DIS_CRC_CHECK                                   7
#define MAC_DIS_CRC_GEN                                     8

#define DEVICE_ID_REG                                        0x0400000
#define DEVICE_REVISION_REG                                  0x0400004
#define DEVICE_STR_REG                                       0x0400008

#define SRAM_BASE_ADDR_REG                                   0x1000000

#define MAC_GRP_0_CONTROL_REG                                0x0600000
#define RX_QUEUE_0_NUM_PKTS_STORED_REG                       0x0600004
#define RX_QUEUE_0_NUM_PKTS_DROPPED_FULL_REG                 0x0600008
#define RX_QUEUE_0_NUM_PKTS_DROPPED_BAD_REG                  0x060000c
#define RX_QUEUE_0_NUM_WORDS_PUSHED_REG                      0x0600010
#define RX_QUEUE_0_NUM_BYTES_PUSHED_REG                      0x0600014
#define RX_QUEUE_0_NUM_PKTS_DEQUEUED_REG                     0x060002c
#define RX_QUEUE_0_NUM_PKTS_IN_QUEUE_REG                     0x0600030
#define TX_QUEUE_0_NUM_PKTS_IN_QUEUE_REG                     0x0600018
#define TX_QUEUE_0_NUM_PKTS_SENT_REG                         0x060001c
#define TX_QUEUE_0_NUM_WORDS_PUSHED_REG                      0x0600020
#define TX_QUEUE_0_NUM_BYTES_PUSHED_REG                      0x0600024
#define TX_QUEUE_0_NUM_PKTS_ENQUEUED_REG                     0x0600028

#define MAC_GRP_1_CONTROL_REG                                0x0640000
#define RX_QUEUE_1_NUM_PKTS_STORED_REG                       0x0640004
#define RX_QUEUE_1_NUM_PKTS_DROPPED_FULL_REG                 0x0640008
#define RX_QUEUE_1_NUM_PKTS_DROPPED_BAD_REG                  0x064000c
#define RX_QUEUE_1_NUM_WORDS_PUSHED_REG                      0x0640010
#define RX_QUEUE_1_NUM_BYTES_PUSHED_REG                      0x0640014
#define RX_QUEUE_1_NUM_PKTS_DEQUEUED_REG                     0x064002c
#define RX_QUEUE_1_NUM_PKTS_IN_QUEUE_REG                     0x0640030
#define TX_QUEUE_1_NUM_PKTS_IN_QUEUE_REG                     0x0640018
#define TX_QUEUE_1_NUM_PKTS_SENT_REG                         0x064001c
#define TX_QUEUE_1_NUM_WORDS_PUSHED_REG                      0x0640020
#define TX_QUEUE_1_NUM_BYTES_PUSHED_REG                      0x0640024
#define TX_QUEUE_1_NUM_PKTS_ENQUEUED_REG                     0x0640028

#define MAC_GRP_2_CONTROL_REG                                0x0680000
#define RX_QUEUE_2_NUM_PKTS_STORED_REG                       0x0680004
#define RX_QUEUE_2_NUM_PKTS_DROPPED_FULL_REG                 0x0680008
#define RX_QUEUE_2_NUM_PKTS_DROPPED_BAD_REG                  0x068000c
#define RX_QUEUE_2_NUM_WORDS_PUSHED_REG                      0x0680010
#define RX_QUEUE_2_NUM_BYTES_PUSHED_REG                      0x0680014
#define RX_QUEUE_2_NUM_PKTS_DEQUEUED_REG                     0x068002c
#define RX_QUEUE_2_NUM_PKTS_IN_QUEUE_REG                     0x0680030
#define TX_QUEUE_2_NUM_PKTS_IN_QUEUE_REG                     0x0680018
#define TX_QUEUE_2_NUM_PKTS_SENT_REG                         0x068001c
#define TX_QUEUE_2_NUM_WORDS_PUSHED_REG                      0x0680020
#define TX_QUEUE_2_NUM_BYTES_PUSHED_REG                      0x0680024
#define TX_QUEUE_2_NUM_PKTS_ENQUEUED_REG                     0x0680028

#define MAC_GRP_3_CONTROL_REG                                0x06c0000
#define RX_QUEUE_3_NUM_PKTS_STORED_REG                       0x06c0004
#define RX_QUEUE_3_NUM_PKTS_DROPPED_FULL_REG                 0x06c0008
#define RX_QUEUE_3_NUM_PKTS_DROPPED_BAD_REG                  0x06c000c
#define RX_QUEUE_3_NUM_WORDS_PUSHED_REG                      0x06c0010
#define RX_QUEUE_3_NUM_BYTES_PUSHED_REG                      0x06c0014
#define RX_QUEUE_3_NUM_PKTS_DEQUEUED_REG                     0x06c002c
#define RX_QUEUE_3_NUM_PKTS_IN_QUEUE_REG                     0x06c0030
#define TX_QUEUE_3_NUM_PKTS_IN_QUEUE_REG                     0x06c0018
#define TX_QUEUE_3_NUM_PKTS_SENT_REG                         0x06c001c
#define TX_QUEUE_3_NUM_WORDS_PUSHED_REG                      0x06c0020
#define TX_QUEUE_3_NUM_BYTES_PUSHED_REG                      0x06c0024
#define TX_QUEUE_3_NUM_PKTS_ENQUEUED_REG                     0x06c0028

#define CPU_REG_Q_0_WR_DATA_WORD_REG                         0x0700000
#define CPU_REG_Q_0_WR_CTRL_WORD_REG                         0x0700004
#define CPU_REG_Q_0_WR_NUM_WORDS_LEFT_REG                    0x0700008
#define CPU_REG_Q_0_WR_NUM_PKTS_IN_Q_REG                     0x070000c
#define CPU_REG_Q_0_RD_DATA_WORD_REG                         0x0700010
#define CPU_REG_Q_0_RD_CTRL_WORD_REG                         0x0700014
#define CPU_REG_Q_0_RD_NUM_WORDS_AVAIL_REG                   0x0700018
#define CPU_REG_Q_0_RD_NUM_PKTS_IN_Q_REG                     0x070001c
#define CPU_REG_Q_0_RX_NUM_PKTS_RCVD_REG                     0x0700020
#define CPU_REG_Q_0_TX_NUM_PKTS_SENT_REG                     0x0700024
#define CPU_REG_Q_0_RX_NUM_WORDS_RCVD_REG                    0x0700028
#define CPU_REG_Q_0_TX_NUM_WORDS_SENT_REG                    0x070002c
#define CPU_REG_Q_0_RX_NUM_BYTES_RCVD_REG                    0x0700030
#define CPU_REG_Q_0_TX_NUM_BYTES_SENT_REG                    0x0700034

#define CPU_REG_Q_1_WR_DATA_WORD_REG                         0x0740000
#define CPU_REG_Q_1_WR_CTRL_WORD_REG                         0x0740004
#define CPU_REG_Q_1_WR_NUM_WORDS_LEFT_REG                    0x0740008
#define CPU_REG_Q_1_WR_NUM_PKTS_IN_Q_REG                     0x074000c
#define CPU_REG_Q_1_RD_DATA_WORD_REG                         0x0740010
#define CPU_REG_Q_1_RD_CTRL_WORD_REG                         0x0740014
#define CPU_REG_Q_1_RD_NUM_WORDS_AVAIL_REG                   0x0740018
#define CPU_REG_Q_1_RD_NUM_PKTS_IN_Q_REG                     0x074001c
#define CPU_REG_Q_1_RX_NUM_PKTS_RCVD_REG                     0x0740020
#define CPU_REG_Q_1_TX_NUM_PKTS_SENT_REG                     0x0740024
#define CPU_REG_Q_1_RX_NUM_WORDS_RCVD_REG                    0x0740028
#define CPU_REG_Q_1_TX_NUM_WORDS_SENT_REG                    0x074002c
#define CPU_REG_Q_1_RX_NUM_BYTES_RCVD_REG                    0x0740030
#define CPU_REG_Q_1_TX_NUM_BYTES_SENT_REG                    0x0740034

#define CPU_REG_Q_2_WR_DATA_WORD_REG                         0x0780000
#define CPU_REG_Q_2_WR_CTRL_WORD_REG                         0x0780004
#define CPU_REG_Q_2_WR_NUM_WORDS_LEFT_REG                    0x0780008
#define CPU_REG_Q_2_WR_NUM_PKTS_IN_Q_REG                     0x078000c
#define CPU_REG_Q_2_RD_DATA_WORD_REG                         0x0780010
#define CPU_REG_Q_2_RD_CTRL_WORD_REG                         0x0780014
#define CPU_REG_Q_2_RD_NUM_WORDS_AVAIL_REG                   0x0780018
#define CPU_REG_Q_2_RD_NUM_PKTS_IN_Q_REG                     0x078001c
#define CPU_REG_Q_2_RX_NUM_PKTS_RCVD_REG                     0x0780020
#define CPU_REG_Q_2_TX_NUM_PKTS_SENT_REG                     0x0780024
#define CPU_REG_Q_2_RX_NUM_WORDS_RCVD_REG                    0x0780028
#define CPU_REG_Q_2_TX_NUM_WORDS_SENT_REG                    0x078002c
#define CPU_REG_Q_2_RX_NUM_BYTES_RCVD_REG                    0x0780030
#define CPU_REG_Q_2_TX_NUM_BYTES_SENT_REG                    0x0780034

#define CPU_REG_Q_3_WR_DATA_WORD_REG                         0x07c0000
#define CPU_REG_Q_3_WR_CTRL_WORD_REG                         0x07c0004
#define CPU_REG_Q_3_WR_NUM_WORDS_LEFT_REG                    0x07c0008
#define CPU_REG_Q_3_WR_NUM_PKTS_IN_Q_REG                     0x07c000c
#define CPU_REG_Q_3_RD_DATA_WORD_REG                         0x07c0010
#define CPU_REG_Q_3_RD_CTRL_WORD_REG                         0x07c0014
#define CPU_REG_Q_3_RD_NUM_WORDS_AVAIL_REG                   0x07c0018
#define CPU_REG_Q_3_RD_NUM_PKTS_IN_Q_REG                     0x07c001c
#define CPU_REG_Q_3_RX_NUM_PKTS_RCVD_REG                     0x07c0020
#define CPU_REG_Q_3_TX_NUM_PKTS_SENT_REG                     0x07c0024
#define CPU_REG_Q_3_RX_NUM_WORDS_RCVD_REG                    0x07c0028
#define CPU_REG_Q_3_TX_NUM_WORDS_SENT_REG                    0x07c002c
#define CPU_REG_Q_3_RX_NUM_BYTES_RCVD_REG                    0x07c0030
#define CPU_REG_Q_3_TX_NUM_BYTES_SENT_REG                    0x07c0034

#define CLK_SYN_0_COUNTER_RESET                              0x0600034
#define CLK_SYN_0_TX_LO                                      0x0600038
#define CLK_SYN_0_TX_HI                                      0x060003c
#define CLK_SYN_0_TX_VALID                                   0x0600040
#define CLK_SYN_0_RX_LO                                      0x0600044
#define CLK_SYN_0_RX_HI                                      0x0600048
#define CLK_SYN_0_RX_VALID                                   0x060004c
#define CLK_SYN_1_COUNTER_RESET                              0x0640034
#define CLK_SYN_1_TX_LO                                      0x0640038
#define CLK_SYN_1_TX_HI                                      0x064003c
#define CLK_SYN_1_TX_VALID                                   0x0640040
#define CLK_SYN_1_RX_LO                                      0x0640044
#define CLK_SYN_1_RX_HI                                      0x0640048
#define CLK_SYN_1_RX_VALID                                   0x064004c
#define CLK_SYN_2_COUNTER_RESET                              0x0680034
#define CLK_SYN_2_TX_LO                                      0x0680038
#define CLK_SYN_2_TX_HI                                      0x068003c
#define CLK_SYN_2_TX_VALID                                   0x0680040
#define CLK_SYN_2_RX_LO                                      0x0680044
#define CLK_SYN_2_RX_HI                                      0x0680048
#define CLK_SYN_2_RX_VALID                                   0x068004c
#define CLK_SYN_3_COUNTER_RESET                              0x06c0034
#define CLK_SYN_3_TX_LO                                      0x06c0038
#define CLK_SYN_3_TX_HI                                      0x06c003c
#define CLK_SYN_3_TX_VALID                                   0x06c0040
#define CLK_SYN_3_RX_LO                                      0x06c0044
#define CLK_SYN_3_RX_HI                                      0x06c0048
#define CLK_SYN_3_RX_VALID                                   0x06c004c
#define DMA_TX_QUE_0_REG                                     0x0480000
#define DMA_TX_QUE_0_LAST_1_BYTE_REG                         0x0480004
#define DMA_TX_QUE_0_LAST_2_BYTE_REG                         0x0480008
#define DMA_TX_QUE_0_LAST_3_BYTE_REG                         0x048000c
#define DMA_TX_QUE_0_LAST_4_BYTE_REG                         0x0480010

#define DMA_TX_QUE_1_REG                                     0x0480040
#define DMA_TX_QUE_1_LAST_1_BYTE_REG                         0x0480044
#define DMA_TX_QUE_1_LAST_2_BYTE_REG                         0x0480048
#define DMA_TX_QUE_1_LAST_3_BYTE_REG                         0x048004c
#define DMA_TX_QUE_1_LAST_4_BYTE_REG                         0x0480050

#define DMA_TX_QUE_2_REG                                     0x0480080
#define DMA_TX_QUE_2_LAST_1_BYTE_REG                         0x0480084
#define DMA_TX_QUE_2_LAST_2_BYTE_REG                         0x0480088
#define DMA_TX_QUE_2_LAST_3_BYTE_REG                         0x048008c
#define DMA_TX_QUE_2_LAST_4_BYTE_REG                         0x0480090

#define DMA_TX_QUE_3_REG                                     0x04800c0
#define DMA_TX_QUE_3_LAST_1_BYTE_REG                         0x04800c4
#define DMA_TX_QUE_3_LAST_2_BYTE_REG                         0x04800c8
#define DMA_TX_QUE_3_LAST_3_BYTE_REG                         0x04800cc
#define DMA_TX_QUE_3_LAST_4_BYTE_REG                         0x04800d0

#define MDIO_0_CONTROL_REG                                   0x04c0000
#define MDIO_0_STATUS_REG                                    0x04c0004
#define MDIO_0_PHY_ID_0_REG                                  0x04c0008
#define MDIO_0_PHY_ID_1_REG                                  0x04c000c
#define MDIO_0_AUTONEGOTIATION_ADVERT_REG                    0x04c0010
#define MDIO_0_AUTONEG_LINK_PARTNER_BASE_PAGE_ABILITY_REG    0x04c0014
#define MDIO_0_AUTONEG_EXPANSION_REG                         0x04c0018
#define MDIO_0_AUTONEG_NEXT_PAGE_TX_REG                      0x04c001c
#define MDIO_0_AUTONEG_LINK_PARTNER_RCVD_NEXT_PAGE_REG       0x04c0020
#define MDIO_0_MASTER_SLAVE_CTRL_REG                         0x04c0024
#define MDIO_0_MASTER_SLAVE_STATUS_REG                       0x04c0028
#define MDIO_0_PSE_CTRL_REG                                  0x04c002c
#define MDIO_0_PSE_STATUS_REG                                0x04c0030
#define MDIO_0_MMD_ACCESS_CTRL_REG                           0x04c0034
#define MDIO_0_MMD_ACCESS_STATUS_REG                         0x04c0038
#define MDIO_0_EXTENDED_STATUS_REG                           0x04c003c

#define MDIO_1_CONTROL_REG                                   0x04c0080
#define MDIO_1_STATUS_REG                                    0x04c0084
#define MDIO_1_PHY_ID_0_REG                                  0x04c0088
#define MDIO_1_PHY_ID_1_REG                                  0x04c008c
#define MDIO_1_AUTONEGOTIATION_ADVERT_REG                    0x04c0090
#define MDIO_1_AUTONEG_LINK_PARTNER_BASE_PAGE_ABILITY_REG    0x04c0094
#define MDIO_1_AUTONEG_EXPANSION_REG                         0x04c0098
#define MDIO_1_AUTONEG_NEXT_PAGE_TX_REG                      0x04c009c
#define MDIO_1_AUTONEG_LINK_PARTNER_RCVD_NEXT_PAGE_REG       0x04c00a0
#define MDIO_1_MASTER_SLAVE_CTRL_REG                         0x04c00a4
#define MDIO_1_MASTER_SLAVE_STATUS_REG                       0x04c00a8
#define MDIO_1_PSE_CTRL_REG                                  0x04c00ac
#define MDIO_1_PSE_STATUS_REG                                0x04c00b0
#define MDIO_1_MMD_ACCESS_CTRL_REG                           0x04c00b4
#define MDIO_1_MMD_ACCESS_STATUS_REG                         0x04c00b8
#define MDIO_1_EXTENDED_STATUS_REG                           0x04c00bc

#define MDIO_2_CONTROL_REG                                   0x04c0100
#define MDIO_2_STATUS_REG                                    0x04c0104
#define MDIO_2_PHY_ID_0_REG                                  0x04c0108
#define MDIO_2_PHY_ID_1_REG                                  0x04c010c
#define MDIO_2_AUTONEGOTIATION_ADVERT_REG                    0x04c0110
#define MDIO_2_AUTONEG_LINK_PARTNER_BASE_PAGE_ABILITY_REG    0x04c0114
#define MDIO_2_AUTONEG_EXPANSION_REG                         0x04c0118
#define MDIO_2_AUTONEG_NEXT_PAGE_TX_REG                      0x04c011c
#define MDIO_2_AUTONEG_LINK_PARTNER_RCVD_NEXT_PAGE_REG       0x04c0120
#define MDIO_2_MASTER_SLAVE_CTRL_REG                         0x04c0124
#define MDIO_2_MASTER_SLAVE_STATUS_REG                       0x04c0128
#define MDIO_2_PSE_CTRL_REG                                  0x04c012c
#define MDIO_2_PSE_STATUS_REG                                0x04c0130
#define MDIO_2_MMD_ACCESS_CTRL_REG                           0x04c0134
#define MDIO_2_MMD_ACCESS_STATUS_REG                         0x04c0138
#define MDIO_2_EXTENDED_STATUS_REG                           0x04c013c

#define MDIO_3_CONTROL_REG                                   0x04c0180
#define MDIO_3_STATUS_REG                                    0x04c0184
#define MDIO_3_PHY_ID_0_REG                                  0x04c0188
#define MDIO_3_PHY_ID_1_REG                                  0x04c018c
#define MDIO_3_AUTONEGOTIATION_ADVERT_REG                    0x04c0190
#define MDIO_3_AUTONEG_LINK_PARTNER_BASE_PAGE_ABILITY_REG    0x04c0194
#define MDIO_3_AUTONEG_EXPANSION_REG                         0x04c0198
#define MDIO_3_AUTONEG_NEXT_PAGE_TX_REG                      0x04c019c
#define MDIO_3_AUTONEG_LINK_PARTNER_RCVD_NEXT_PAGE_REG       0x04c01a0
#define MDIO_3_MASTER_SLAVE_CTRL_REG                         0x04c01a4
#define MDIO_3_MASTER_SLAVE_STATUS_REG                       0x04c01a8
#define MDIO_3_PSE_CTRL_REG                                  0x04c01ac
#define MDIO_3_PSE_STATUS_REG                                0x04c01b0
#define MDIO_3_MMD_ACCESS_CTRL_REG                           0x04c01b4
#define MDIO_3_MMD_ACCESS_STATUS_REG                         0x04c01b8
#define MDIO_3_EXTENDED_STATUS_REG                           0x04c01bc

#define STAMP_COUNTER_1_REG                                  0x0400000
#define STAMP_COUNTER_2_REG                                  0x0400004
#define STAMP_COUNTER_3_REG                                  0x0400008
#define STAMP_COUNTER_4_REG                                  0x040000c
#define STAMP_COUNTER_1_2_REG_LOAD                           0x0400010
#define STAMP_COUNTER_3_4_REG_LOAD                           0x0400014
#define PHY_RST_BIT_POS                              15
#define PHY_LOOPBACK_BIT_POS                         14
#define PHY_SPEED_SEL_LO_BIT_POS                     13
#define PHY_AUTONEG_ENABLE_BIT_POS                   12
#define PHY_PWR_DOWN_BIT_POS                         11
#define PHY_ISOLATE_BIT_POS                          10
#define PHY_RESTART_AUTONEG_BIT_POS                   9
#define PHY_DUPLEX_MODE_BIT_POS                       8
#define PHY_COLLISION_TEST_BIT_POS                    7
#define PHY_SPEED_SEL_HI_BIT_POS                      6
#define PHY_UNIDIR_ENABLE_BIT_POS                     5

#define PHY_100BASE_T4_BIT_POS                       15
#define PHY_100BASE_X_FULL_DPLX_BIT_POS              14
#define PHY_100BASE_X_HALF_DPLX_BIT_POS              13
#define PHY_10MBPS_FULL_DPLX_BIT_POS                 12
#define PHY_10MBPS_HALF_DPLX_BIT_POS                 11
#define PHY_100BASE_T2_FULL_DPLX_BIT_POS             10
#define PHY_100BASE_T2_HALF_DPLX_BIT_POS              9
#define PHY_EXTENDED_STATUS_BIT_POS                   8
#define PHY_UNIDIR_ABILITY_BIT_POS                    7
#define PHY_MF_PREAMBLE_SPRSN_BIT_POS                 6
#define PHY_AUTONEG_COMPLETE_BIT_POS                  5
#define PHY_REMOTE_FAULT_BIT_POS                      4
#define PHY_AUTONEG_ABILITY_BIT_POS                   3
#define PHY_LINK_STATUS_BIT_POS                       2
#define PHY_JABBER_DETECT_BIT_POS                     1
#define PHY_EXTENDED_CAPABILITY_BIT_POS               0

#define IN_ARB_NUM_PKTS_SENT_REG                  0x2000200
#define IN_ARB_LAST_PKT_WORD_0_LO_REG             0x2000204
#define IN_ARB_LAST_PKT_WORD_0_HI_REG             0x2000208
#define IN_ARB_LAST_PKT_CTRL_0_REG                0x200020c
#define IN_ARB_LAST_PKT_WORD_1_LO_REG             0x2000210
#define IN_ARB_LAST_PKT_WORD_1_HI_REG             0x2000214
#define IN_ARB_LAST_PKT_CTRL_1_REG                0x2000218
#define IN_ARB_STATE_REG                          0x200021c

#define SWITCH_OP_LUT_PORTS_MAC_HI_REG            0x2000100
#define SWITCH_OP_LUT_MAC_LO_REG                  0x2000104
#define SWITCH_OP_LUT_NUM_HITS_REG                0x2000108
#define SWITCH_OP_LUT_NUM_MISSES_REG              0x200010c
#define SWITCH_OP_LUT_MAC_LUT_RD_ADDR_REG         0x2000110
#define SWITCH_OP_LUT_MAC_LUT_WR_ADDR_REG         0x2000114

#define ROUTER_RT_SIZE                                     32
#define ROUTER_ARP_SIZE                                    32
#define ROUTER_DST_IP_FILTER_TABLE_DEPTH                   32

#define ROUTER_OP_LUT_ARP_MAC_HI_REG              0x2000100
#define ROUTER_OP_LUT_ARP_MAC_LO_REG              0x2000104
#define ROUTER_OP_LUT_ARP_NEXT_HOP_IP_REG         0x2000108
#define ROUTER_OP_LUT_ARP_LUT_RD_ADDR_REG         0x200010c
#define ROUTER_OP_LUT_ARP_LUT_WR_ADDR_REG         0x2000110
#define ROUTER_OP_LUT_RT_IP_REG                   0x2000114
#define ROUTER_OP_LUT_RT_MASK_REG                 0x2000118
#define ROUTER_OP_LUT_RT_NEXT_HOP_IP_REG          0x200011c
#define ROUTER_OP_LUT_RT_OUTPUT_PORT_REG          0x2000120
#define ROUTER_OP_LUT_RT_LUT_RD_ADDR_REG          0x2000124
#define ROUTER_OP_LUT_RT_LUT_WR_ADDR_REG          0x2000128
#define ROUTER_OP_LUT_MAC_0_HI_REG                0x200012c
#define ROUTER_OP_LUT_MAC_0_LO_REG                0x2000130
#define ROUTER_OP_LUT_MAC_1_HI_REG                0x2000134
#define ROUTER_OP_LUT_MAC_1_LO_REG                0x2000138
#define ROUTER_OP_LUT_MAC_2_HI_REG                0x200013c
#define ROUTER_OP_LUT_MAC_2_LO_REG                0x2000140
#define ROUTER_OP_LUT_MAC_3_HI_REG                0x2000144
#define ROUTER_OP_LUT_MAC_3_LO_REG                0x2000148
#define ROUTER_OP_LUT_DST_IP_FILTER_IP_REG        0x200014c
#define ROUTER_OP_LUT_DST_IP_FILTER_RD_ADDR_REG   0x2000150
#define ROUTER_OP_LUT_DST_IP_FILTER_WR_ADDR_REG   0x2000154

#define ROUTER_OP_LUT_ARP_NUM_MISSES_REG          0x2000180
#define ROUTER_OP_LUT_LPM_NUM_MISSES_REG          0x2000184
#define ROUTER_OP_LUT_NUM_CPU_PKTS_SENT_REG       0x2000188
#define ROUTER_OP_LUT_NUM_BAD_OPTS_VER_REG        0x200018c
#define ROUTER_OP_LUT_NUM_BAD_CHKSUMS_REG         0x2000190
#define ROUTER_OP_LUT_NUM_BAD_TTLS_REG            0x2000194
#define ROUTER_OP_LUT_NUM_NON_IP_RCVD_REG         0x2000198
#define ROUTER_OP_LUT_NUM_PKTS_FORWARDED_REG      0x200019c
#define ROUTER_OP_LUT_NUM_WRONG_DEST_REG          0x20001a0
#define ROUTER_OP_LUT_NUM_FILTERED_PKTS_REG       0x20001a4

#define OQ_ENABLE_SEND_BIT_NUM                              0
#define OQ_INITIALIZE_OQ_BIT_NUM                            1

#define OQ_NUM_WORDS_LEFT_REG_0                 0x2101000
#define OQ_NUM_PKT_BYTES_STORED_REG_0           0x2101004
#define OQ_NUM_OVERHEAD_BYTES_STORED_REG_0      0x2101008
#define OQ_NUM_PKTS_STORED_REG_0                0x210100c
#define OQ_NUM_PKTS_DROPPED_REG_0               0x2101010
#define OQ_NUM_PKT_BYTES_REMOVED_REG_0          0x2101014
#define OQ_NUM_OVERHEAD_BYTES_REMOVED_REG_0     0x2101018
#define OQ_NUM_PKTS_REMOVED_REG_0               0x210101c
#define OQ_ADDRESS_HI_REG_0                     0x2101020
#define OQ_ADDRESS_LO_REG_0                     0x2101024
#define OQ_WR_ADDRESS_REG_0                     0x2101028
#define OQ_RD_ADDRESS_REG_0                     0x210102c
#define OQ_NUM_PKTS_IN_Q_REG_0                  0x2101030
#define OQ_MAX_PKTS_IN_Q_REG_0                  0x2101034
#define OQ_FULL_THRESH_REG_0                    0x2101040
#define OQ_NUM_WORDS_IN_Q_REG_0                 0x210103c
#define OQ_CONTROL_REG_0                        0x2101038

#define OQ_NUM_WORDS_LEFT_REG_1                 0x2101100
#define OQ_NUM_PKT_BYTES_STORED_REG_1           0x2101104
#define OQ_NUM_OVERHEAD_BYTES_STORED_REG_1      0x2101108
#define OQ_NUM_PKTS_STORED_REG_1                0x210110c
#define OQ_NUM_PKTS_DROPPED_REG_1               0x2101110
#define OQ_NUM_PKT_BYTES_REMOVED_REG_1          0x2101114
#define OQ_NUM_OVERHEAD_BYTES_REMOVED_REG_1     0x2101118
#define OQ_NUM_PKTS_REMOVED_REG_1               0x210111c
#define OQ_ADDRESS_HI_REG_1                     0x2101120
#define OQ_ADDRESS_LO_REG_1                     0x2101124
#define OQ_WR_ADDRESS_REG_1                     0x2101128
#define OQ_RD_ADDRESS_REG_1                     0x210112c
#define OQ_NUM_PKTS_IN_Q_REG_1                  0x2101130
#define OQ_MAX_PKTS_IN_Q_REG_1                  0x2101134
#define OQ_FULL_THRESH_REG_1                    0x2101140
#define OQ_NUM_WORDS_IN_Q_REG_1                 0x210113c
#define OQ_CONTROL_REG_1                        0x2101138

#define OQ_NUM_WORDS_LEFT_REG_2                 0x2101200
#define OQ_NUM_PKT_BYTES_STORED_REG_2           0x2101204
#define OQ_NUM_OVERHEAD_BYTES_STORED_REG_2      0x2101208
#define OQ_NUM_PKTS_STORED_REG_2                0x210120c
#define OQ_NUM_PKTS_DROPPED_REG_2               0x2101210
#define OQ_NUM_PKT_BYTES_REMOVED_REG_2          0x2101214
#define OQ_NUM_OVERHEAD_BYTES_REMOVED_REG_2     0x2101218
#define OQ_NUM_PKTS_REMOVED_REG_2               0x210121c
#define OQ_ADDRESS_HI_REG_2                     0x2101220
#define OQ_ADDRESS_LO_REG_2                     0x2101224
#define OQ_WR_ADDRESS_REG_2                     0x2101228
#define OQ_RD_ADDRESS_REG_2                     0x210122c
#define OQ_NUM_PKTS_IN_Q_REG_2                  0x2101230
#define OQ_MAX_PKTS_IN_Q_REG_2                  0x2101234
#define OQ_FULL_THRESH_REG_2                    0x2101240
#define OQ_NUM_WORDS_IN_Q_REG_2                 0x210123c
#define OQ_CONTROL_REG_2                        0x2101238

#define OQ_NUM_WORDS_LEFT_REG_3                 0x2101300
#define OQ_NUM_PKT_BYTES_STORED_REG_3           0x2101304
#define OQ_NUM_OVERHEAD_BYTES_STORED_REG_3      0x2101308
#define OQ_NUM_PKTS_STORED_REG_3                0x210130c
#define OQ_NUM_PKTS_DROPPED_REG_3               0x2101310
#define OQ_NUM_PKT_BYTES_REMOVED_REG_3          0x2101314
#define OQ_NUM_OVERHEAD_BYTES_REMOVED_REG_3     0x2101318
#define OQ_NUM_PKTS_REMOVED_REG_3               0x210131c
#define OQ_ADDRESS_HI_REG_3                     0x2101320
#define OQ_ADDRESS_LO_REG_3                     0x2101324
#define OQ_WR_ADDRESS_REG_3                     0x2101328
#define OQ_RD_ADDRESS_REG_3                     0x210132c
#define OQ_NUM_PKTS_IN_Q_REG_3                  0x2101330
#define OQ_MAX_PKTS_IN_Q_REG_3                  0x2101334
#define OQ_FULL_THRESH_REG_3                    0x2101340
#define OQ_NUM_WORDS_IN_Q_REG_3                 0x210133c
#define OQ_CONTROL_REG_3                        0x2101338

#define OQ_NUM_WORDS_LEFT_REG_4                 0x2101400
#define OQ_NUM_PKT_BYTES_STORED_REG_4           0x2101404
#define OQ_NUM_OVERHEAD_BYTES_STORED_REG_4      0x2101408
#define OQ_NUM_PKTS_STORED_REG_4                0x210140c
#define OQ_NUM_PKTS_DROPPED_REG_4               0x2101410
#define OQ_NUM_PKT_BYTES_REMOVED_REG_4          0x2101414
#define OQ_NUM_OVERHEAD_BYTES_REMOVED_REG_4     0x2101418
#define OQ_NUM_PKTS_REMOVED_REG_4               0x210141c
#define OQ_ADDRESS_HI_REG_4                     0x2101420
#define OQ_ADDRESS_LO_REG_4                     0x2101424
#define OQ_WR_ADDRESS_REG_4                     0x2101428
#define OQ_RD_ADDRESS_REG_4                     0x210142c
#define OQ_NUM_PKTS_IN_Q_REG_4                  0x2101430
#define OQ_MAX_PKTS_IN_Q_REG_4                  0x2101434
#define OQ_FULL_THRESH_REG_4                    0x2101440
#define OQ_NUM_WORDS_IN_Q_REG_4                 0x210143c
#define OQ_CONTROL_REG_4                        0x2101438

#define OQ_NUM_WORDS_LEFT_REG_5                 0x2101500
#define OQ_NUM_PKT_BYTES_STORED_REG_5           0x2101504
#define OQ_NUM_OVERHEAD_BYTES_STORED_REG_5      0x2101508
#define OQ_NUM_PKTS_STORED_REG_5                0x210150c
#define OQ_NUM_PKTS_DROPPED_REG_5               0x2101510
#define OQ_NUM_PKT_BYTES_REMOVED_REG_5          0x2101514
#define OQ_NUM_OVERHEAD_BYTES_REMOVED_REG_5     0x2101518
#define OQ_NUM_PKTS_REMOVED_REG_5               0x210151c
#define OQ_ADDRESS_HI_REG_5                     0x2101520
#define OQ_ADDRESS_LO_REG_5                     0x2101524
#define OQ_WR_ADDRESS_REG_5                     0x2101528
#define OQ_RD_ADDRESS_REG_5                     0x210152c
#define OQ_NUM_PKTS_IN_Q_REG_5                  0x2101530
#define OQ_MAX_PKTS_IN_Q_REG_5                  0x2101534
#define OQ_FULL_THRESH_REG_5                    0x2101540
#define OQ_NUM_WORDS_IN_Q_REG_5                 0x210153c
#define OQ_CONTROL_REG_5                        0x2101538

#define OQ_NUM_WORDS_LEFT_REG_6                 0x2101600
#define OQ_NUM_PKT_BYTES_STORED_REG_6           0x2101604
#define OQ_NUM_OVERHEAD_BYTES_STORED_REG_6      0x2101608
#define OQ_NUM_PKTS_STORED_REG_6                0x210160c
#define OQ_NUM_PKTS_DROPPED_REG_6               0x2101610
#define OQ_NUM_PKT_BYTES_REMOVED_REG_6          0x2101614
#define OQ_NUM_OVERHEAD_BYTES_REMOVED_REG_6     0x2101618
#define OQ_NUM_PKTS_REMOVED_REG_6               0x210161c
#define OQ_ADDRESS_HI_REG_6                     0x2101620
#define OQ_ADDRESS_LO_REG_6                     0x2101624
#define OQ_WR_ADDRESS_REG_6                     0x2101628
#define OQ_RD_ADDRESS_REG_6                     0x210162c
#define OQ_NUM_PKTS_IN_Q_REG_6                  0x2101630
#define OQ_MAX_PKTS_IN_Q_REG_6                  0x2101634
#define OQ_FULL_THRESH_REG_6                    0x2101640
#define OQ_NUM_WORDS_IN_Q_REG_6                 0x210163c
#define OQ_CONTROL_REG_6                        0x2101638

#define OQ_NUM_WORDS_LEFT_REG_7                 0x2101700
#define OQ_NUM_PKT_BYTES_STORED_REG_7           0x2101704
#define OQ_NUM_OVERHEAD_BYTES_STORED_REG_7      0x2101708
#define OQ_NUM_PKTS_STORED_REG_7                0x210170c
#define OQ_NUM_PKTS_DROPPED_REG_7               0x2101710
#define OQ_NUM_PKT_BYTES_REMOVED_REG_7          0x2101714
#define OQ_NUM_OVERHEAD_BYTES_REMOVED_REG_7     0x2101718
#define OQ_NUM_PKTS_REMOVED_REG_7               0x210171c
#define OQ_ADDRESS_HI_REG_7                     0x2101720
#define OQ_ADDRESS_LO_REG_7                     0x2101724
#define OQ_WR_ADDRESS_REG_7                     0x2101728
#define OQ_RD_ADDRESS_REG_7                     0x210172c
#define OQ_NUM_PKTS_IN_Q_REG_7                  0x2101730
#define OQ_MAX_PKTS_IN_Q_REG_7                  0x2101734
#define OQ_FULL_THRESH_REG_7                    0x2101740
#define OQ_NUM_WORDS_IN_Q_REG_7                 0x210173c
#define OQ_CONTROL_REG_7                        0x2101738

#define DELAY_ENABLE_REG                        0x2000500
#define DELAY_1ST_WORD_HI_REG                   0x2000508
#define DELAY_1ST_WORD_LO_REG                   0x200050c
#define DELAY_LENGTH_REG                        0x2000504

#define RATE_LIMIT_ENABLE_REG                   0x2000400
#define RATE_LIMIT_SHIFT_REG                    0x2000404

#define RCP_RATE_REG                            0x2107000
#define RCP_MAC_0_RTT_L_REG                     0x2107004
#define RCP_MAC_0_RTT_H_REG                     0x2107008
#define RCP_MAC_0_NUM_RCP_REG                   0x2107014
#define RCP_MAC_0_NUM_BYTE_H_REG                0x2107010
#define RCP_MAC_0_NUM_BYTE_L_REG                0x210700c
#define RCP_MAC_1_RTT_L_REG                     0x2107018
#define RCP_MAC_1_RTT_H_REG                     0x210701c
#define RCP_MAC_1_NUM_RCP_REG                   0x2107028
#define RCP_MAC_1_NUM_BYTE_H_REG                0x2107024
#define RCP_MAC_1_NUM_BYTE_L_REG                0x2107020
#define RCP_MAC_2_RTT_L_REG                     0x210702c
#define RCP_MAC_2_RTT_H_REG                     0x2107030
#define RCP_MAC_2_NUM_BYTE_L_REG                0x2107034
#define RCP_MAC_2_NUM_BYTE_H_REG                0x2107038
#define RCP_MAC_2_NUM_RCP_REG                   0x210703c
#define RCP_MAC_3_RTT_L_REG                     0x2107040
#define RCP_MAC_3_RTT_H_REG                     0x2107044
#define RCP_MAC_3_NUM_BYTE_L_REG                0x2107048
#define RCP_MAC_3_NUM_BYTE_H_REG                0x210704c
#define RCP_MAC_3_NUM_RCP_REG                   0x2107050

#define EVT_CAP_ENABLE_CAPTURE_REG              0x2000300
#define EVT_CAP_SEND_PKT_REG                    0x2000304
#define EVT_CAP_DST_MAC_HI_REG                  0x2000308
#define EVT_CAP_DST_MAC_LO_REG                  0x200030c
#define EVT_CAP_SRC_MAC_HI_REG                  0x2000310
#define EVT_CAP_SRC_MAC_LO_REG                  0x2000314
#define EVT_CAP_ETHERTYPE_REG                   0x2000318
#define EVT_CAP_IP_DST_REG                      0x200031c
#define EVT_CAP_IP_SRC_REG                      0x2000320
#define EVT_CAP_UDP_SRC_PORT_REG                0x2000330
#define EVT_CAP_UDP_DST_PORT_REG                0x2000334
#define EVT_CAP_OUTPUT_PORTS_REG                0x2000338
#define EVT_CAP_RESET_TIMERS_REG                0x200033c
#define EVT_CAP_MONITOR_MASK_REG                0x2000324
#define EVT_CAP_TIMER_RESOLUTION_REG            0x2000340
#define EVT_CAP_NUM_EVT_PKTS_SENT_REG           0x2000344
#define EVT_CAP_NUM_EVTS_SENT_REG               0x2000348
#define EVT_CAP_NUM_EVTS_DROPPED_REG            0x200032c
#define EVT_CAP_SIGNAL_ID_MASK_REG              0x2000328

#define BRAM_OQ_NUM_PKT_BYTES_RECEIVED_0_REG    0x2000700
#define BRAM_OQ_NUM_PKTS_RECEIVED_0_REG         0x2000704
#define BRAM_OQ_NUM_PKTS_DROPPED_0_REG          0x2000708
#define BRAM_OQ_NUM_PKT_BYTES_RECEIVED_1_REG    0x200070c
#define BRAM_OQ_NUM_PKTS_RECEIVED_1_REG         0x2000710
#define BRAM_OQ_NUM_PKTS_DROPPED_1_REG          0x2000714
#define BRAM_OQ_NUM_PKT_BYTES_RECEIVED_2_REG    0x2000718
#define BRAM_OQ_NUM_PKTS_RECEIVED_2_REG         0x200071c
#define BRAM_OQ_NUM_PKTS_DROPPED_2_REG          0x2000720
#define BRAM_OQ_NUM_PKT_BYTES_RECEIVED_3_REG    0x2000724
#define BRAM_OQ_NUM_PKTS_RECEIVED_3_REG         0x2000728
#define BRAM_OQ_NUM_PKTS_DROPPED_3_REG          0x200072c
#define BRAM_OQ_NUM_PKT_BYTES_RECEIVED_4_REG    0x2000730
#define BRAM_OQ_NUM_PKTS_RECEIVED_4_REG         0x2000734
#define BRAM_OQ_NUM_PKTS_DROPPED_4_REG          0x2000738
#define BRAM_OQ_NUM_PKT_BYTES_RECEIVED_5_REG    0x200073c
#define BRAM_OQ_NUM_PKTS_RECEIVED_5_REG         0x2000740
#define BRAM_OQ_NUM_PKTS_DROPPED_5_REG          0x2000744
#define BRAM_OQ_NUM_PKT_BYTES_RECEIVED_6_REG    0x2000748
#define BRAM_OQ_NUM_PKTS_RECEIVED_6_REG         0x200074c
#define BRAM_OQ_NUM_PKTS_DROPPED_6_REG          0x2000750
#define BRAM_OQ_NUM_PKT_BYTES_RECEIVED_7_REG    0x2000754
#define BRAM_OQ_NUM_PKTS_RECEIVED_7_REG         0x2000758
#define BRAM_OQ_NUM_PKTS_DROPPED_7_REG          0x200075c
#define BRAM_OQ_DISABLE_QUEUES_REG              0x2000760
#define BRAM_OQ_NUM_WORDS_IN_QUEUE_0_REG        0x2000764
#define BRAM_OQ_NUM_WORDS_IN_QUEUE_1_REG        0x2000768
#define BRAM_OQ_NUM_WORDS_IN_QUEUE_2_REG        0x200076c
#define BRAM_OQ_NUM_WORDS_IN_QUEUE_3_REG        0x2000770
#define BRAM_OQ_NUM_WORDS_IN_QUEUE_4_REG        0x2000774
#define BRAM_OQ_NUM_WORDS_IN_QUEUE_5_REG        0x2000778
#define BRAM_OQ_NUM_WORDS_IN_QUEUE_6_REG        0x200077c
#define BRAM_OQ_NUM_WORDS_IN_QUEUE_7_REG        0x2000780

#define OPENFLOW_WILDCARD_TABLE_SIZE                        32
#define OPENFLOW_WILDCARD_NUM_DATA_WORDS_USED               10
#define OPENFLOW_WILDCARD_NUM_CMP_WORDS_USED                 8

#define OPENFLOW_ENTRY_WIDTH                               240
#define OPENFLOW_ENTRY_TRANSP_DST_WIDTH                     16
#define OPENFLOW_ENTRY_TRANSP_DST_POS                        0
#define OPENFLOW_ENTRY_TRANSP_SRC_WIDTH                     16
#define OPENFLOW_ENTRY_TRANSP_SRC_POS                       16
#define OPENFLOW_ENTRY_IP_PROTO_WIDTH                        8
#define OPENFLOW_ENTRY_IP_PROTO_POS                         32
#define OPENFLOW_ENTRY_IP_DST_WIDTH                         32
#define OPENFLOW_ENTRY_IP_DST_POS                           40
#define OPENFLOW_ENTRY_IP_SRC_WIDTH                         32
#define OPENFLOW_ENTRY_IP_SRC_POS                           72
#define OPENFLOW_ENTRY_VLAN_ID_WIDTH                        16
#define OPENFLOW_ENTRY_VLAN_ID_POS                         224
#define OPENFLOW_ENTRY_ETH_TYPE_WIDTH                       16
#define OPENFLOW_ENTRY_ETH_TYPE_POS                        104
#define OPENFLOW_ENTRY_ETH_DST_WIDTH                        48
#define OPENFLOW_ENTRY_ETH_DST_POS                         120
#define OPENFLOW_ENTRY_ETH_SRC_WIDTH                        48
#define OPENFLOW_ENTRY_ETH_SRC_POS                         168
#define OPENFLOW_ENTRY_SRC_PORT_WIDTH                        8
#define OPENFLOW_ENTRY_SRC_PORT_POS                        216

#define OPENFLOW_ACTION_WIDTH                              320
#define OPENFLOW_FORWARD_BITMASK_WIDTH                      16
#define OPENFLOW_FORWARD_BITMASK_POS                         0
#define OPENFLOW_PKT_TRIM_ACTION_WIDTH                      16
#define OPENFLOW_PKT_TRIM_ACTION_POS                        16
#define OPENFLOW_REPLACEMENT_ACTION_WIDTH                   24
#define OPENFLOW_NUM_REPLACEMENT_ACTIONS                     6
#define OPENFLOW_REPLACEMENT_ACTION_POS                     48
#define OPENFLOW_REPLACEMENT_BITMASK_ACTION_WIDTH           40
#define OPENFLOW_NUM_REPLACEMENT_BITMASK_ACTIONS             2
#define OPENFLOW_REPLACEMENT_BITMASK_ACTION_POS            192
#define OPENFLOW_INCREMENT_ACTION_WIDTH                     24
#define OPENFLOW_NUM_INCREMENT_ACTIONS                       2
#define OPENFLOW_INCREMENT_ACTION_POS                      192
#define OPENFLOW_VLAN_MOD_ACTION_WIDTH                      16

#define OPENFLOW_REPLACEMENT_ACTION_VLD_BIT                 23
#define OPENFLOW_REPLACEMENT_ACTION_WORD_SEL_START          20
#define OPENFLOW_REPLACEMENT_ACTION_WORD_SEL_LEN             3
#define OPENFLOW_REPLACEMENT_ACTION_2B_H_1B_L_BIT           16
#define OPENFLOW_REPLACEMENT_ACTION_NEW_DATA_WIDTH          16
#define OPENFLOW_REPLACEMENT_ACTION_CTRL               0x10

#define OPENFLOW_INCREMENT_ACTION_VLD_BIT                   23
#define OPENFLOW_INCREMENT_ACTION_WORD_SEL_START            20
#define OPENFLOW_INCREMENT_ACTION_WORD_SEL_LEN               3
#define OPENFLOW_INCREMENT_ACTION_ADDEND_WIDTH              16
#define OPENFLOW_INCREMENT_ACTION_CTRL                 0x20

#define OPENFLOW_VLAN_MOD_ACTION_SEL_POS                    12
#define OPENFLOW_VLAN_MOD_ACTION_SEL_WIDTH                   4
#define OPENFLOW_VLAN_MOD_ACTION_CTRL                  0x40

#define VLAN_MOD_DO_NOTHING                            0x0
#define VLAN_MOD_ADD                                   0x1
#define VLAN_MOD_MODIFY                                0x2
#define VLAN_MOD_ADD_MODIFY                            0x3
#define VLAN_MOD_REMOVE                                0x4

#define OPENFLOW_EXACT_ENTRY_HDR_BASE_POS                    0
#define OPENFLOW_EXACT_ENTRY_COUNTERS_POS                    8
#define OPENFLOW_EXACT_ENTRY_ACTION_BASE_POS                10

#define OPENFLOW_EXACT_ENTRY_PKT_COUNTER_WIDTH              25
#define OPENFLOW_EXACT_ENTRY_BYTE_COUNTER_WIDTH             32
#define OPENFLOW_EXACT_ENTRY_LAST_SEEN_WIDTH                 7
#define OPENFLOW_EXACT_ENTRY_PKT_COUNTER_POS                 0
#define OPENFLOW_EXACT_ENTRY_BYTE_COUNTER_POS               32
#define OPENFLOW_EXACT_ENTRY_LAST_SEEN_POS                  25

#define OPENFLOW_LOOKUP_WILDCARD_MISSES_REG            0x2000800
#define OPENFLOW_LOOKUP_WILDCARD_HITS_REG              0x2000804
#define OPENFLOW_LOOKUP_EXACT_MISSES_REG               0x2000808
#define OPENFLOW_LOOKUP_EXACT_HITS_REG                 0x200080c
#define OPENFLOW_LOOKUP_NUM_PKTS_DROPPED_0_REG         0x2000810
#define OPENFLOW_LOOKUP_NUM_PKTS_DROPPED_1_REG         0x2000814
#define OPENFLOW_LOOKUP_NUM_PKTS_DROPPED_2_REG         0x2000818
#define OPENFLOW_LOOKUP_NUM_PKTS_DROPPED_3_REG         0x200081c
#define OPENFLOW_LOOKUP_NUM_PKTS_DROPPED_4_REG         0x2000820
#define OPENFLOW_LOOKUP_NUM_PKTS_DROPPED_5_REG         0x2000824
#define OPENFLOW_LOOKUP_NUM_PKTS_DROPPED_6_REG         0x2000828
#define OPENFLOW_LOOKUP_NUM_PKTS_DROPPED_7_REG         0x200082c
#define OPENFLOW_LOOKUP_TIMER_REG                      0x2000838

#define OPENFLOW_WILDCARD_LOOKUP_READ_ADDR_REG         0x2103068
#define OPENFLOW_WILDCARD_LOOKUP_WRITE_ADDR_REG        0x210306c
#define OPENFLOW_WILDCARD_LOOKUP_ACTION_BASE_REG       0x2103000
#define OPENFLOW_WILDCARD_LOOKUP_CMP_BASE_REG          0x2103048
#define OPENFLOW_WILDCARD_LOOKUP_CMP_MASK_BASE_REG     0x2103028
#define OPENFLOW_WILDCARD_LOOKUP_BYTES_HIT_BASE_REG    0x2103070
#define OPENFLOW_WILDCARD_LOOKUP_PKTS_HIT_BASE_REG     0x21030f0
#define OPENFLOW_WILDCARD_LOOKUP_LAST_SEEN_TS_REG      0x2103178

#endif

