
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


/*
*  nginx数据结构  动态数组
*/

#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct {
    void        *elts;      //指向数组首地址
    ngx_uint_t   nelts;     //表示数组中已经使用的元素个数
    size_t       size;      //每个数组元素占用的内存大小
    ngx_uint_t   nalloc;    //当前数组中容纳元素的总大小
    ngx_pool_t  *pool;      //nginx内存池对象
} ngx_array_t;

/*
*   创建一个动态数组,并预分配N个大小为size 的内存空间
*   @p:     nginx内存池对象
*   @n:     初始分配元素的最大个数
*   @size:  每个元素占用的内存大小
*   @return: 返回一个分配好内存的nginx 动态数组对象
*/
ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);


/*
*   销毁已经分配的数组元素空间 和nginx 动态数组对象
*   @a: nginx 动态数组对象
*/
void ngx_array_destroy(ngx_array_t *a);


/*
*   向动态数组对象中添加一个元素,并返回新增元素指针的首地址
*   @a:      nginx 动态数组对象    
*   @return  新增元素指针的首地址
*/
void *ngx_array_push(ngx_array_t *a);

/*
*   向动态数组对象中添加N个元素,并返回新增元素指针的首地址
*   @a:     nginx 动态数组对象   
*   @n:     新增元素的个数
*   @return  新增元素指针的首地址
*/
void *ngx_array_push_n(ngx_array_t *a, ngx_uint_t n);

/*
*   初始化一个已经存在的内存数组,并预分配n个大小为size的内存空间
*   @a:     动态数组的结构体指针
*   @p:     nginx内存池对象
*   @n:     初始分配元素的最大个数
*   @size:  每个元素占用的内存大小
*   @return: 返回一个分配好内存的nginx 动态数组对象
*/
static ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->size = size;
    array->nalloc = n;
    array->pool = pool;

    array->elts = ngx_palloc(pool, n * size);
    if (array->elts == NULL) {
        return NGX_ERROR;
    }

    return NGX_OK;
}


#endif /* _NGX_ARRAY_H_INCLUDED_ */
