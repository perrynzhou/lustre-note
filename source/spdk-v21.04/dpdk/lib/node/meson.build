# SPDX-License-Identifier: BSD-3-Clause
# Copyright(C) 2020 Marvell International Ltd.

sources = files(
        'ethdev_ctrl.c',
        'ethdev_rx.c',
        'ethdev_tx.c',
        'ip4_lookup.c',
        'ip4_rewrite.c',
        'log.c',
        'null.c',
        'pkt_cls.c',
        'pkt_drop.c',
)
headers = files('rte_node_ip4_api.h', 'rte_node_eth_api.h')
# Strict-aliasing rules are violated by uint8_t[] to context size casts.
cflags += '-fno-strict-aliasing'
deps += ['graph', 'mbuf', 'lpm', 'ethdev', 'mempool', 'cryptodev']
build = false
reason = 'not needed by SPDK'
