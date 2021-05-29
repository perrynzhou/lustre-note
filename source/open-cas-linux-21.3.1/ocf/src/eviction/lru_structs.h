/*
 * Copyright(c) 2012-2021 Intel Corporation
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */
#ifndef __EVICTION_LRU_STRUCTS_H__

#define __EVICTION_LRU_STRUCTS_H__

struct lru_eviction_policy_meta {
	uint32_t prev;
	uint32_t next;
	uint8_t hot;
} __attribute__((packed));

struct ocf_lru_list {
	uint32_t num_nodes;
	uint32_t head;
	uint32_t tail;
	uint32_t num_hot;
	uint32_t last_hot;
};

struct lru_eviction_policy {
	struct ocf_lru_list clean;
	struct ocf_lru_list dirty;
};

#define OCF_LRU_HOT_RATIO 2

#endif
