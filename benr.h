#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Operate on bencoded data directly, without any decoding
 */

enum benr_kind {
	BENR_STRING,
	BENR_INT,
	BENR_DICT,
	BENR_LIST,

	/* error-likes */
	/* TODO: consider moving these to return values? */
	BENR_NONE,

	BENR_ERR_UNEXPECTED_BYTE,
	BENR_ERR_UNEXPECTED_EOF,
	BENR_ERR_UNEXPECTED_BYTE_IN_INT,
	BENR_ERR_UNEXPECTED_EOF_IN_INT,
	BENR_ERR_STRING_TOO_LONG,

	/* internal */
	BENR_UINT,
};

struct benr_string {
	unsigned char *start;
	size_t len;
};

struct benr_ctx {
	void *start;
	size_t len;
};

struct benr_dict {
	/* note that ctx.len is max bytes, not strict bound */
	struct benr_ctx ctx;
};

struct benr_list {
	/* note that ctx.len is max bytes, not strict bound */
	struct benr_ctx ctx;
};

struct benr {
	enum benr_kind kind;
	union {
		struct benr_string s;
		intmax_t i;
		struct benr_dict d;
		struct benr_list l;

		/* error-likes */
		void *error_loc;
		uintmax_t error_len;

		/* internal */
		uintmax_t u;
	} data;
};

void benr_init(struct benr *b, void *data, size_t data_bytes);

bool benr_is_string(struct benr *b);
bool benr_is_int(struct benr *b);
bool benr_is_dict(struct benr *b);
bool benr_is_list(struct benr *b);

int benr_as_string(struct benr *b, struct benr_string *s);
int benr_as_int(struct benr *b, intmax_t *s);
int benr_as_dict(struct benr *b, struct benr_dict *s);
int benr_as_list(struct benr *b, struct benr_list *s);


struct benr_dict_iter {

};

struct benr_list_iter {

};

void benr_list_iter(struct benr_list *l, struct benr_list_iter *i);
void benr_dict_iter(struct benr_dict *l, struct benr_dict_iter *i);

bool benr_list_iter_has_next(struct benr_list_iter *l);
bool benr_dict_iter_has_next(struct benr_dict_iter *l);

int benr_list_iter_next(struct benr_list_iter *l, struct benr *b);
int benr_dict_iter_next(struct benr_dict_iter *l, struct benr_string *key, struct benr *val);