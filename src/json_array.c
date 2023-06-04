#include "json.h"
#include "json_array.h"

struct json_array_t*
json_array_create()
{
  struct json_array_t* array = calloc(1, sizeof(*array));
  if (!array)
    return NULL;

  array->n_items = 0;
  array->item_capacity = 10;
  array->item_types = calloc(array->item_capacity, sizeof(*array->item_types));
  if (!array->item_types)
  {
    free(array);
    return NULL;
  }

  array->current_bytes = 0;
  array->byte_capacity = 100;
  array->items = calloc(array->byte_capacity, 1);

  if (!array->items)
  {
    free(array->item_types);
    free(array);
    return NULL;
  }

  return array;
}

bool
json_array_append(
  struct json_array_t* array,
  enum json_type_e item_type,
  void* item,
  size_t sizeof_item)
{
  /* APPEND ITEM TYPE */
  array->item_types[array->n_items++] = item_type;
  if (array->n_items == array->item_capacity)
  {
    size_t new_item_capacity = array->item_capacity * 2;
    void* alloc = realloc(array->item_types, new_item_capacity * sizeof(*array->item_types));
    if (!alloc)
      return false;
    array->item_capacity = new_item_capacity;
    array->item_types = alloc;
  }

  /* APPEND ITEM */
  if (sizeof_item + array->current_bytes >= array->byte_capacity)
  {
    size_t new_byte_capacity = array->byte_capacity *= 2;
    void *alloc = realloc(array->items, new_byte_capacity);
    if (!alloc)
      return false;
    array->byte_capacity = new_byte_capacity;
    array->items = alloc;
  }

  void* write_to = (char*)array->items + array->current_bytes;
  memcpy(write_to, item, sizeof_item);
  array->current_bytes += sizeof_item;

  return true;
}

void*
json_array_get_mixed(
  const struct json_array_t* const array,
  const size_t idx)
{
  size_t offset = 0;
  for (size_t i = 0; i < idx; ++i)
    offset += json_type_to_size(array->item_types[i]);

  return (char*)array->items + offset;
}

void*
json_array_get_fixed(
  const struct json_array_t* const array,
  const size_t idx,
  const size_t sizeof_item)
{
  size_t offset = idx * sizeof_item;
  return (char*)array->items + offset;
}

void
json_array_free(
  struct json_array_t** array)
{

}
