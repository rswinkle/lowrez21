#ifndef CVECTOR_uvec3_H
#define CVECTOR_uvec3_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Data structure for uvec3 vector. */
typedef struct cvector_uvec3
{
	uvec3* a;           /**< Array. */
	size_t size;       /**< Current size (amount you use when manipulating array directly). */
	size_t capacity;   /**< Allocated size of array; always >= size. */
} cvector_uvec3;



extern size_t CVEC_uvec3_SZ;

int cvec_uvec3(cvector_uvec3* vec, size_t size, size_t capacity);
int cvec_init_uvec3(cvector_uvec3* vec, uvec3* vals, size_t num);

cvector_uvec3* cvec_uvec3_heap(size_t size, size_t capacity);
cvector_uvec3* cvec_init_uvec3_heap(uvec3* vals, size_t num);
int cvec_copyc_uvec3(void* dest, void* src);
int cvec_copy_uvec3(cvector_uvec3* dest, cvector_uvec3* src);

int cvec_push_uvec3(cvector_uvec3* vec, uvec3 a);
uvec3 cvec_pop_uvec3(cvector_uvec3* vec);

int cvec_extend_uvec3(cvector_uvec3* vec, size_t num);
int cvec_insert_uvec3(cvector_uvec3* vec, size_t i, uvec3 a);
int cvec_insert_array_uvec3(cvector_uvec3* vec, size_t i, uvec3* a, size_t num);
uvec3 cvec_replace_uvec3(cvector_uvec3* vec, size_t i, uvec3 a);
void cvec_erase_uvec3(cvector_uvec3* vec, size_t start, size_t end);
int cvec_reserve_uvec3(cvector_uvec3* vec, size_t size);
int cvec_set_cap_uvec3(cvector_uvec3* vec, size_t size);
void cvec_set_val_sz_uvec3(cvector_uvec3* vec, uvec3 val);
void cvec_set_val_cap_uvec3(cvector_uvec3* vec, uvec3 val);

uvec3* cvec_back_uvec3(cvector_uvec3* vec);

void cvec_clear_uvec3(cvector_uvec3* vec);
void cvec_free_uvec3_heap(void* vec);
void cvec_free_uvec3(void* vec);

#ifdef __cplusplus
}
#endif

/* CVECTOR_uvec3_H */
#endif


#ifdef CVECTOR_uvec3_IMPLEMENTATION

size_t CVEC_uvec3_SZ = 50;

#define CVEC_uvec3_ALLOCATOR(x) ((x+1) * 2)


#if defined(CVEC_MALLOC) && defined(CVEC_FREE) && defined(CVEC_REALLOC)
/* ok */
#elif !defined(CVEC_MALLOC) && !defined(CVEC_FREE) && !defined(CVEC_REALLOC)
/* ok */
#else
#error "Must define all or none of CVEC_MALLOC, CVEC_FREE, and CVEC_REALLOC."
#endif

#ifndef CVEC_MALLOC
#define CVEC_MALLOC(sz)      malloc(sz)
#define CVEC_REALLOC(p, sz)  realloc(p, sz)
#define CVEC_FREE(p)         free(p)
#endif

#ifndef CVEC_MEMMOVE
#include <string.h>
#define CVEC_MEMMOVE(dst, src, sz)  memmove(dst, src, sz)
#endif

#ifndef CVEC_ASSERT
#include <assert.h>
#define CVEC_ASSERT(x)       assert(x)
#endif

cvector_uvec3* cvec_uvec3_heap(size_t size, size_t capacity)
{
	cvector_uvec3* vec;
	if (!(vec = (cvector_uvec3*)CVEC_MALLOC(sizeof(cvector_uvec3)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_uvec3_SZ;

	if (!(vec->a = (uvec3*)CVEC_MALLOC(vec->capacity*sizeof(uvec3)))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}

	return vec;
}

cvector_uvec3* cvec_init_uvec3_heap(uvec3* vals, size_t num)
{
	cvector_uvec3* vec;
	
	if (!(vec = (cvector_uvec3*)CVEC_MALLOC(sizeof(cvector_uvec3)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->capacity = num + CVEC_uvec3_SZ;
	vec->size = num;
	if (!(vec->a = (uvec3*)CVEC_MALLOC(vec->capacity*sizeof(uvec3)))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}

	CVEC_MEMMOVE(vec->a, vals, sizeof(uvec3)*num);

	return vec;
}

int cvec_uvec3(cvector_uvec3* vec, size_t size, size_t capacity)
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_uvec3_SZ;

	if (!(vec->a = (uvec3*)CVEC_MALLOC(vec->capacity*sizeof(uvec3)))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	return 1;
}

int cvec_init_uvec3(cvector_uvec3* vec, uvec3* vals, size_t num)
{
	vec->capacity = num + CVEC_uvec3_SZ;
	vec->size = num;
	if (!(vec->a = (uvec3*)CVEC_MALLOC(vec->capacity*sizeof(uvec3)))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	CVEC_MEMMOVE(vec->a, vals, sizeof(uvec3)*num);

	return 1;
}

int cvec_copyc_uvec3(void* dest, void* src)
{
	cvector_uvec3* vec1 = (cvector_uvec3*)dest;
	cvector_uvec3* vec2 = (cvector_uvec3*)src;

	vec1->a = NULL;
	vec1->size = 0;
	vec1->capacity = 0;

	return cvec_copy_uvec3(vec1, vec2);
}

int cvec_copy_uvec3(cvector_uvec3* dest, cvector_uvec3* src)
{
	uvec3* tmp = NULL;
	if (!(tmp = (uvec3*)CVEC_REALLOC(dest->a, src->capacity*sizeof(uvec3)))) {
		CVEC_ASSERT(tmp != NULL);
		return 0;
	}
	dest->a = tmp;

	CVEC_MEMMOVE(dest->a, src->a, src->size*sizeof(uvec3));
	dest->size = src->size;
	dest->capacity = src->capacity;
	return 1;
}


int cvec_push_uvec3(cvector_uvec3* vec, uvec3 a)
{
	uvec3* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		vec->a[vec->size++] = a;
	} else {
		tmp_sz = CVEC_uvec3_ALLOCATOR(vec->capacity);
		if (!(tmp = (uvec3*)CVEC_REALLOC(vec->a, sizeof(uvec3)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->a[vec->size++] = a;
		vec->capacity = tmp_sz;
	}
	return 1;
}

uvec3 cvec_pop_uvec3(cvector_uvec3* vec)
{
	return vec->a[--vec->size];
}

uvec3* cvec_back_uvec3(cvector_uvec3* vec)
{
	return &vec->a[vec->size-1];
}

int cvec_extend_uvec3(cvector_uvec3* vec, size_t num)
{
	uvec3* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_uvec3_SZ;
		if (!(tmp = (uvec3*)CVEC_REALLOC(vec->a, sizeof(uvec3)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	vec->size += num;
	return 1;
}

int cvec_insert_uvec3(cvector_uvec3* vec, size_t i, uvec3 a)
{
	uvec3* tmp;
	size_t tmp_sz;
	if (vec->capacity > vec->size) {
		CVEC_MEMMOVE(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(uvec3));
		vec->a[i] = a;
	} else {
		tmp_sz = CVEC_uvec3_ALLOCATOR(vec->capacity);
		if (!(tmp = (uvec3*)CVEC_REALLOC(vec->a, sizeof(uvec3)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		CVEC_MEMMOVE(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(uvec3));
		vec->a[i] = a;
		vec->capacity = tmp_sz;
	}

	vec->size++;
	return 1;
}

int cvec_insert_array_uvec3(cvector_uvec3* vec, size_t i, uvec3* a, size_t num)
{
	uvec3* tmp;
	size_t tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_uvec3_SZ;
		if (!(tmp = (uvec3*)CVEC_REALLOC(vec->a, sizeof(uvec3)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	CVEC_MEMMOVE(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(uvec3));
	CVEC_MEMMOVE(&vec->a[i], a, num*sizeof(uvec3));
	vec->size += num;
	return 1;
}

uvec3 cvec_replace_uvec3(cvector_uvec3* vec, size_t i, uvec3 a)
{
	uvec3 tmp = vec->a[i];
	vec->a[i] = a;
	return tmp;
}

void cvec_erase_uvec3(cvector_uvec3* vec, size_t start, size_t end)
{
	size_t d = end - start + 1;
	CVEC_MEMMOVE(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(uvec3));
	vec->size -= d;
}


int cvec_reserve_uvec3(cvector_uvec3* vec, size_t size)
{
	uvec3* tmp;
	if (vec->capacity < size) {
		if (!(tmp = (uvec3*)CVEC_REALLOC(vec->a, sizeof(uvec3)*(size+CVEC_uvec3_SZ)))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + CVEC_uvec3_SZ;
	}
	return 1;
}

int cvec_set_cap_uvec3(cvector_uvec3* vec, size_t size)
{
	uvec3* tmp;
	if (size < vec->size) {
		vec->size = size;
	}

	if (!(tmp = (uvec3*)CVEC_REALLOC(vec->a, sizeof(uvec3)*size))) {
		CVEC_ASSERT(tmp != NULL);
		return 0;
	}
	vec->a = tmp;
	vec->capacity = size;
	return 1;
}

void cvec_set_val_sz_uvec3(cvector_uvec3* vec, uvec3 val)
{
	size_t i;
	for (i=0; i<vec->size; i++) {
		vec->a[i] = val;
	}
}

void cvec_set_val_cap_uvec3(cvector_uvec3* vec, uvec3 val)
{
	size_t i;
	for (i=0; i<vec->capacity; i++) {
		vec->a[i] = val;
	}
}

void cvec_clear_uvec3(cvector_uvec3* vec) { vec->size = 0; }

void cvec_free_uvec3_heap(void* vec)
{
	cvector_uvec3* tmp = (cvector_uvec3*)vec;
	if (!tmp) return;
	CVEC_FREE(tmp->a);
	CVEC_FREE(tmp);
}

void cvec_free_uvec3(void* vec)
{
	cvector_uvec3* tmp = (cvector_uvec3*)vec;
	CVEC_FREE(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}

#endif
