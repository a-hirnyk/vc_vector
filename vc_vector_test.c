#include "vc_vector_test.h"
#include <stdlib.h>
#include <string.h>
#include "vc_vector.h"

#define ASSERT_EQ(expected, actual) if ((expected) != (actual)) { \
                                      printf("Failed passing line %u. Expected: %u. Actual: %u.\n", \
                                             __LINE__, (int)(expected), (int)(actual)); \
                                      abort(); \
                                    }

#define ASSERT_NE(not_expected, actual) if ((not_expected) == (actual)) { \
                                          printf("Failed passing line %u. Non expected actual value: %u.\n", \
                                                 __LINE__, (int)(actual)); \
                                          abort(); \
                                        }

#define ASSERT_TRUE(actual) ASSERT_EQ(true, (actual));

#define ASSERT_FALSE(actual) ASSERT_EQ(false, (actual));

#define PRINT_VECTOR(vector, type, format) for (void* i = vc_vector_begin(vector); \
                                                      i != vc_vector_end(vector); \
                                                      i = vc_vector_next(vector, i)) { \
                                                  printf(format, *(type*)i); \
                                           } \
                                           printf("\n");

#define PRINT_VECTOR_INT(vector) PRINT_VECTOR(vector, int, "%u; ")
#define PRINT_VECTOR_STR(vector) PRINT_VECTOR(vector, char *, "%s; ");

// ----------------------------------------------------------------------------

void test_vc_vector_create() {
  const size_t size_of_type = sizeof(int);
  const size_t default_count_of_elements = vc_vector_get_default_count_of_elements();
  
  // Creating vector with default count of elements
  vc_vector* vector = vc_vector_create(0, size_of_type, NULL);
  ASSERT_NE(NULL, vector);
  ASSERT_EQ(0, vc_vector_count(vector));
  ASSERT_EQ(0, vc_vector_size(vector));
  ASSERT_EQ(default_count_of_elements, vc_vector_max_count(vector));
  ASSERT_EQ(size_of_type * default_count_of_elements, vc_vector_max_size(vector));
  vc_vector_release(vector);
  
  // Creating vector with custom count of elements
  const size_t test_count_of_elements = 7;
  vector = vc_vector_create(test_count_of_elements, size_of_type, NULL);
  ASSERT_NE(NULL, vector);
  ASSERT_EQ(0, vc_vector_count(vector));
  ASSERT_EQ(0, vc_vector_size(vector));
  ASSERT_EQ(test_count_of_elements, vc_vector_max_count(vector));
  ASSERT_EQ(size_of_type * test_count_of_elements, vc_vector_max_size(vector));
  vc_vector_release(vector);
  
  // Creating vector with zero size of single element
  vector = vc_vector_create(0, 0, NULL);
  ASSERT_EQ(NULL, vector);
  
  // Creating copy of vector
  vector = vc_vector_create(0, size_of_type, NULL);
  ASSERT_NE(NULL, vector);
  for (int i = 0; i < test_count_of_elements; ++i) {
    ASSERT_TRUE(vc_vector_push_back(vector, &i));
  }
  
  vc_vector* vector_copy = vc_vector_create_copy(vector);
  ASSERT_NE(NULL, vector_copy);
  ASSERT_TRUE(vc_vector_is_equals(vector, vector_copy));
  
  vc_vector_release(vector_copy);
  vc_vector_release(vector);
  
  printf("%s passed.\n", __PRETTY_FUNCTION__);
}

void test_vc_vector_element_access() {
  const int test_num_start = 18;
  const int test_num_end = 36;
  const size_t size_of_type = sizeof(test_num_start);
  
  vc_vector* vector = vc_vector_create(0, size_of_type, NULL);
  ASSERT_NE(0, vector);
  for (int i = test_num_start; i <= test_num_end; ++i) {
    ASSERT_TRUE(vc_vector_push_back(vector, &i));
  }
  
  ASSERT_EQ(test_num_start, *(int*)vc_vector_front(vector));
  ASSERT_EQ(test_num_start, *(int*)vc_vector_data(vector));
  ASSERT_EQ(test_num_end, *(int*)vc_vector_back(vector));
  
  for (int i = test_num_start, j = 0; i <= test_num_end; ++i, ++j) {
    ASSERT_EQ(i, *(int*)vc_vector_at(vector, j));
  }
  
  vc_vector_release(vector);
  
  printf("%s passed.\n", __PRETTY_FUNCTION__);
}

void test_vc_vector_iterators() {
  vc_vector* vector = vc_vector_create(0, sizeof(int), NULL);
  ASSERT_NE(NULL, vector);
  
  const int test_count_of_elements = 23;
  for (int i = 0; i < test_count_of_elements; ++i) {
    ASSERT_EQ(true, vc_vector_push_back(vector, &i));
  }
  
  int j = 0;
  for (void* i = vc_vector_begin(vector); i != vc_vector_end(vector); i = vc_vector_next(vector, i)) {
    ASSERT_EQ(j++, *(int*)i);
  }
  
  ASSERT_EQ(test_count_of_elements, j);
  vc_vector_release(vector);
  
  printf("%s passed.\n", __PRETTY_FUNCTION__);
}

void test_vc_vector_capacity() {
  const int size_of_element = sizeof(int);
  const float growth_factor = vc_vector_get_growth_factor();
  ASSERT_EQ(1.5, growth_factor);
  
  const int count_of_elements_initialized = 22;
  const int max_size_of_vector_initialized = count_of_elements_initialized * size_of_element;
  const int count_of_elements_ended = 23;
  const int size_of_vector_ended = count_of_elements_ended * size_of_element;
  const int max_count_of_vector_ended = count_of_elements_initialized * growth_factor;
  const int max_size_of_vector_ended = max_count_of_vector_ended * size_of_element;
  
  vc_vector* vector = vc_vector_create(count_of_elements_initialized, size_of_element, NULL);
  ASSERT_NE(NULL, vector);
  
  ASSERT_EQ(0, vc_vector_count(vector));
  ASSERT_EQ(true, vc_vector_empty(vector));
  ASSERT_EQ(0, vc_vector_size(vector));
  ASSERT_EQ(count_of_elements_initialized, vc_vector_max_count(vector));
  ASSERT_EQ(max_size_of_vector_initialized, vc_vector_max_size(vector));
  
  for (int i = 0; i < count_of_elements_ended; ++i) {
    ASSERT_EQ(true, vc_vector_push_back(vector, &i));
  }
  
  ASSERT_EQ(count_of_elements_ended, vc_vector_count(vector));
  ASSERT_EQ(false, vc_vector_empty(vector));
  ASSERT_EQ(size_of_vector_ended, vc_vector_size(vector));
  ASSERT_EQ(max_count_of_vector_ended, vc_vector_max_count(vector));
  ASSERT_EQ(max_size_of_vector_ended, vc_vector_max_size(vector));

  const int test_reserve_count_fail = 10;
  ASSERT_EQ(false, vc_vector_reserve_count(vector, test_reserve_count_fail));
  
  const int test_reserve_count = 35;
  ASSERT_EQ(true, vc_vector_reserve_count(vector, test_reserve_count));
  ASSERT_EQ(test_reserve_count, vc_vector_max_count(vector));
  ASSERT_EQ(test_reserve_count * size_of_element, vc_vector_max_size(vector));
  
  // Second time with the same value
  ASSERT_EQ(true, vc_vector_reserve_count(vector, test_reserve_count));
  ASSERT_EQ(test_reserve_count, vc_vector_max_count(vector));
  ASSERT_EQ(test_reserve_count * size_of_element, vc_vector_max_size(vector));
  
  const int test_reserve_size = 123 * size_of_element;
  ASSERT_EQ(true, vc_vector_reserve_size(vector, test_reserve_size));
  ASSERT_EQ(test_reserve_size / size_of_element, vc_vector_max_count(vector));
  ASSERT_EQ(test_reserve_size, vc_vector_max_size(vector));
  
  vc_vector_release(vector);
  
  printf("%s passed.\n", __PRETTY_FUNCTION__);
}

void test_vc_vector_modifiers() {
  const int begin[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20
  };
  const int size_of_element = sizeof(begin[0]);
  const int count_of_elements = sizeof(begin) / size_of_element;
  
  // After deleting first, last and one middle elements
  const int after_deleting_some_elements[] = {
    /* 1, */ 2, 3, 4, 5, 6, 7, 8, 9, 10, /* 11, */ 12, 13, 14, 15, 16, 17, 18, 19, /* 20 */
  };
  
  // After deleting first 3 elemets from begin, 4 from middle and 3 from end
  const int after_deleting_some_ranges[] = {
    /* 1, 2, 3, */ 4, 5, 6, 7, 8, /* 9, 10, 11, 12, */ 13, 14, 15, 16, 17, /* 18, 19, 20 */
  };
  
  vc_vector* vector = vc_vector_create(0, size_of_element, NULL);
  ASSERT_NE(NULL, vector);
  
  // Append test
  
  ASSERT_EQ(true, vc_vector_append(vector, (void*)begin, count_of_elements));
  
  ASSERT_EQ(count_of_elements, vc_vector_count(vector));
  for (int i = 0; i < vc_vector_count(vector); ++i) {
    ASSERT_EQ(begin[i], *(int*)vc_vector_at(vector, i));
  }
  
  // Pop back test
  
  while (vc_vector_count(vector) > 0) {
    ASSERT_EQ(true, vc_vector_pop_back(vector));
  }
  
  ASSERT_EQ(true, vc_vector_empty(vector));
  
  // Push back test
  
  for (int i = 0; i < count_of_elements; ++i) {
    ASSERT_EQ(true, vc_vector_push_back(vector, (void*)&begin[i]));
  }
  
  ASSERT_EQ(count_of_elements, vc_vector_count(vector));
  for (int i = 0; i < vc_vector_count(vector); ++i) {
    ASSERT_EQ(begin[i], *(int*)vc_vector_at(vector, i));
  }
  
  // Erase test
  
  vc_vector_clear(vector);
  ASSERT_EQ(true, vc_vector_append(vector, (void*)begin, count_of_elements));
  
  ASSERT_EQ(true, vc_vector_erase(vector, 0));
  ASSERT_EQ(true, vc_vector_erase(vector, vc_vector_count(vector) - 1));
  ASSERT_EQ(true, vc_vector_erase(vector, vc_vector_count(vector) / 2));
  
  ASSERT_EQ(sizeof(after_deleting_some_elements) / size_of_element, vc_vector_count(vector));
  for (int i = 0; i < vc_vector_count(vector); ++i) {
    ASSERT_EQ(after_deleting_some_elements[i], *(int*)vc_vector_at(vector, i));
  }
  
  // Erase range test
  
  vc_vector_clear(vector);
  ASSERT_EQ(true, vc_vector_append(vector, (void*)begin, count_of_elements));
  
  ASSERT_EQ(true, vc_vector_erase_range(vector, 0, 3));
  ASSERT_EQ(true, vc_vector_erase_range(vector, vc_vector_count(vector) - 3, vc_vector_count(vector)));
  ASSERT_EQ(true, vc_vector_erase_range(vector, vc_vector_count(vector) / 2 - 2, vc_vector_count(vector) / 2 + 2));
  
  ASSERT_EQ(sizeof(after_deleting_some_ranges) / size_of_element, vc_vector_count(vector));
  for (int i = 0; i < vc_vector_count(vector); ++i) {
    ASSERT_EQ(after_deleting_some_ranges[i], *(int*)vc_vector_at(vector, i));
  }
  
  // Insert test
  
  vc_vector_clear(vector);
  for (int i = 1; i < count_of_elements - 1; ++i) {
    ASSERT_EQ(true, vc_vector_insert(vector, i - 1, (void*)&begin[i]));
  }
  
  ASSERT_EQ(true, vc_vector_insert(vector, 0, &begin[0]));
  ASSERT_EQ(true, vc_vector_insert(vector, vc_vector_count(vector), &begin[count_of_elements - 1]));
  
  ASSERT_EQ(count_of_elements, vc_vector_count(vector));
  for (int i = 0; i < vc_vector_count(vector); ++i) {
    ASSERT_EQ(begin[i], *(int*)vc_vector_at(vector, i));
  }
  
  vc_vector_release(vector);
  
  printf("%s passed.\n", __PRETTY_FUNCTION__);
}

void test_vc_vector_strfreefunc(void *data) {
  free(*(char **)data);
}

void test_vc_vector_with_strfreefunc() {
  // creates a vector of pointers to char, i.e. a vector of variable sized strings
  vc_vector* vector = vc_vector_create(3, sizeof(char *), test_vc_vector_strfreefunc);
  ASSERT_NE(NULL, vector);

  char *strs[] = {
    strdup("abcde"),
    strdup("edcba"),
    strdup("1234554321"),
    strdup("!@#$%"),
    strdup("not empty string"),
    strdup(""),
    strdup("Hello World"),
    strdup("xxxxx"),
    strdup("yyyyy")
  };
  
  for (int i = 0; i < 3; ++i) {
    ASSERT_TRUE(vc_vector_push_back(vector, &strs[i]));
  }
  
  ASSERT_EQ(3, vc_vector_count(vector));

  for (int i = 3; i < 6; ++i) {
    ASSERT_TRUE(vc_vector_insert(vector, i, &strs[i]));
  }
  
  ASSERT_EQ(6, vc_vector_count(vector));
  vc_vector_clear(vector); // strs[0-6] were freed
  ASSERT_EQ(0, vc_vector_count(vector));
  
  for (int i = 6; i < 9; ++i) {
    ASSERT_TRUE(vc_vector_push_back(vector, &strs[i]));
  }
  
  ASSERT_EQ(3, vc_vector_count(vector));

  vc_vector_release(vector);
  
  printf("%s passed.\n", __PRETTY_FUNCTION__);
}

void vc_vector_run_tests() {
  test_vc_vector_create();
  test_vc_vector_element_access();
  test_vc_vector_iterators();
  test_vc_vector_capacity();
  test_vc_vector_modifiers();
  test_vc_vector_with_strfreefunc();
}

int main() {
 vc_vector_run_tests();
 printf("Tests passed.\n");
 return 0;
}
