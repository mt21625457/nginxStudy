
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


/*
*   nginx数据结构  红黑树
*/

#ifndef _NGX_RBTREE_H_INCLUDED_
#define _NGX_RBTREE_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef ngx_uint_t  ngx_rbtree_key_t;
typedef ngx_int_t   ngx_rbtree_key_int_t;


typedef struct ngx_rbtree_node_s  ngx_rbtree_node_t;

/* 红黑树节点的数据结构 */
struct ngx_rbtree_node_s {
    ngx_rbtree_key_t       key;         //无符号整形的关键字
    ngx_rbtree_node_t     *left;        //左子节点
    ngx_rbtree_node_t     *right;       //由子节点
    ngx_rbtree_node_t     *parent;      //父节点
    u_char                 color;       //节点颜色： 0表示黑色  1表示红色
    u_char                 data;        //1个节点数据，一般很少用到
};


typedef struct ngx_rbtree_s  ngx_rbtree_t;

typedef void (*ngx_rbtree_insert_pt) (ngx_rbtree_node_t *root,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);

/* 红黑树数据结构 */
struct ngx_rbtree_s {
    ngx_rbtree_node_t     *root;        //树的根节点
    ngx_rbtree_node_t     *sentinel;    //指向NULL 的哨兵节点
    ngx_rbtree_insert_pt   insert;      //表示红黑树添加元素的函数指针，他会决定添加新节点 是 新增还是 替换
};

/* 
*   初始化一颗红黑树
*   @tree   红黑树容器指针
*   @s      哨兵节点指针
*   @i      表示新增元素的函数指针，即新增元素的回调函数
*/
#define ngx_rbtree_init(tree, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (tree)->root = s;                                                         \
    (tree)->sentinel = s;                                                     \
    (tree)->insert = i


/*
*   向红黑树中添加节点，该方法会通过旋转保持树的平衡
*   @tree   红黑树容器指针
*   @node   需要添加到红黑树中节点
*/
void ngx_rbtree_insert(ngx_rbtree_t *tree, ngx_rbtree_node_t *node);

/*
*   向红黑树中删除节点，该方法会通过旋转保持树的平衡
*   @tree   红黑树容器指针
*   @node   需要删除的 红黑树中的节点 
*/
void ngx_rbtree_delete(ngx_rbtree_t *tree, ngx_rbtree_node_t *node);

/*
*   向红黑树中添加数据节点，每个数据节点都是唯一的
*   @root       红黑树容器指针
*   @node       需要添加到红黑树中节点
*   @sentinel   这颗红黑树 初始化哨兵节点的指针
*/
void ngx_rbtree_insert_value(ngx_rbtree_node_t *root, ngx_rbtree_node_t *node,
    ngx_rbtree_node_t *sentinel);

/*
*   向红黑树中添加数据节点，每个数据节点都是唯一的
*   @root       红黑树容器指针
*   @node       需要添加到红黑树中节点，他对应的关键字时时间或者时间差
                有可能是个负数
*   @sentinel   这颗红黑树 初始化哨兵节点的指针
*/
void ngx_rbtree_insert_timer_value(ngx_rbtree_node_t *root,
    ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel);


/*
*   这个后续注释 暂时没看懂干什么的
*   @tree       红黑树容器指针
*   @node       红黑树中节点
*/
ngx_rbtree_node_t *ngx_rbtree_next(ngx_rbtree_t *tree,
    ngx_rbtree_node_t *node);


#define ngx_rbt_red(node)               ((node)->color = 1)
#define ngx_rbt_black(node)             ((node)->color = 0)
#define ngx_rbt_is_red(node)            ((node)->color)
#define ngx_rbt_is_black(node)          (!ngx_rbt_is_red(node))
#define ngx_rbt_copy_color(n1, n2)      (n1->color = n2->color)


/* a sentinel must be black */

#define ngx_rbtree_sentinel_init(node)  ngx_rbt_black(node)

/*
*   查找下一个节点，利用它可以实现正序遍历红黑树 根据key从小到大
*   @node       当前node节点
*   @sentinel   这颗红黑树 哨兵节点的指针
*/
static ngx_inline ngx_rbtree_node_t *
ngx_rbtree_min(ngx_rbtree_node_t *node, ngx_rbtree_node_t *sentinel)
{
    while (node->left != sentinel) {
        node = node->left;
    }

    return node;
}


#endif /* _NGX_RBTREE_H_INCLUDED_ */
