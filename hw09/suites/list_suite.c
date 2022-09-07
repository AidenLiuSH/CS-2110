#include <stdio.h>
#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "test_utils.h"

// The number of mallocs in user code until the next one fails, or -1 for
// successful mallocs
int mallocs_until_fail = -1;

static void reset_mallocs_until_fail(void)
{
    mallocs_until_fail = -1;
}

// static struct user sample_student = {3, (double[]){59.9, 60.1, 100}};
// static struct instructor sample_instructor = {0.001};
static struct user sample_user_student = { NULL, "Reemas", STUDENT, {.student = {3, (double[]){59.9, 60.1, 100}}} };
// static struct user sample_user_instructor = { NULL, "Feebas", INSTRUCTOR, (union user_data) {.instructor = {0.001}} };
static struct user_list *_create_length_1_list(void)
{
    struct user_list *shortList = _create_list();
    union user_data user_data = {.student = {3, (double[]){59.9, 60.1, 100}}};
    _push_front(shortList, _create_user("Reemas", STUDENT, user_data));
    return shortList;
}

static struct user sample_users[] = {
    { NULL, "Nrehtous", STUDENT, {.student = {3, (double[]){59.9, 60.1, 100}}} },
    { NULL, "Reemas", INSTRUCTOR, {.instructor = {0.001}} },
    { NULL, "Timud", STUDENT, {.student = {3, (double[]){95.2, 32.1, 54.5}}} },
    { NULL, "Balec", INSTRUCTOR, {.instructor = {5555.9}} }
};

static struct user_list *_create_longer_list(void)
{
    struct user_list *longerList = _create_list();
    // Push in reverse order
    _push_front(longerList, _create_user("Balec", INSTRUCTOR, (union user_data) {.instructor = {5555.9}}));
    _push_front(longerList, _create_user("Timud", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    _push_front(longerList, _create_user("Reemas", INSTRUCTOR, (union user_data) {.instructor = {0.001}}));
    _push_front(longerList, _create_user("Nrehtous", STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}));
    return longerList;
}

void checkListEquals(struct user_list *curList, struct user_list *original, char *msg)
{
    // assumes that curList and original are non-null
    ck_assert_msg(curList->size == original->size, msg);
    struct user *curUser = curList->head;
    struct user *curOrigUser = original->head;
    for (int i = 0; i < curList->size; i++) {
        ck_assert_msg(curUser != NULL, msg);
        ck_assert_msg(_user_equal(curUser, curOrigUser), msg);

        curUser = curUser->next;
        curOrigUser = curOrigUser->next;
    }
    ck_assert_msg(curUser == NULL, msg);
}

/******************************************************************************/
/************************** create_list() tests *******************************/
/******************************************************************************/

START_TEST(test_list_create_list_basic)
{
    struct user_list *l = create_list();
    ck_assert_msg(l, "create_list should not return NULL");
    ck_assert_msg(!l->size, "create_list should return a list of size 0");
    ck_assert_msg(!l->head, "create_list should return a list with no head");
    free(l);
}
END_TEST

START_TEST(test_list_create_list_malloc_failure)
{
    mallocs_until_fail = 0;
    struct user_list *l = create_list();
    ck_assert_msg(!l, "create_list improperly handles malloc failure");
}
END_TEST

/******************************************************************************/
/************************** push_front() tests ********************************/
/******************************************************************************/

START_TEST(test_list_push_front_NULL_list)
{
    ck_assert_msg(push_front(NULL, "Reemas", STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}) != SUCCESS, "should not be able to push to a NULL list");
}
END_TEST

START_TEST(test_list_push_front_malloc_failure)
{
    struct user_list *l = _create_list();
    mallocs_until_fail = 0;

    int result = push_front(l, "Reemas", STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}});
    ck_assert_msg(result == FAILURE, "push_front should return FAILURE on malloc failure");

    _free_list(l);
}
END_TEST

START_TEST(test_list_push_front_NULL_name_student)
{
    struct user_list *l = _create_list();

    struct user *user = _create_user(NULL, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}});

    ck_assert_msg(push_front(l, NULL, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}) == SUCCESS, "successful push_front should return SUCCESS");
    ck_assert_msg(l->size == 1, "push_front should increment size");
    ck_assert_msg(l->head, "push_front should add a node");
    ck_assert_msg(!l->head->next, "you may be adding extra nodes somehow");
    ck_assert_msg(_user_equal(l->head, user), "push_front adds wrong data (double-check your create methods as well!)");

    _free_user(user);
    _free_list(l);
}
END_TEST

START_TEST(test_list_push_front_NULL_name_instructor)
{
    struct user_list *l = _create_list();

    struct user *user = _create_user(NULL, INSTRUCTOR, (union user_data) {.instructor = {0.001}});

    ck_assert_msg(push_front(l, NULL, INSTRUCTOR, (union user_data) {.instructor = {0.001}}) == SUCCESS, "successful push_front should return SUCCESS");
    ck_assert_msg(l->size == 1, "push_front should increment size");
    ck_assert_msg(l->head, "push_front should add a node");
    ck_assert_msg(!l->head->next, "you may be adding extra nodes somehow");
    ck_assert_msg(_user_equal(l->head, user), "push_front adds wrong data (double-check your create methods as well!)");

    _free_user(user);
    _free_list(l);
}
END_TEST

START_TEST(test_list_push_front_empty_student)
{
    struct user_list *l = _create_list();

    char *name = "Teetu";
    struct user *user = _create_user(name, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}});

    ck_assert_msg(push_front(l, name, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}) == SUCCESS, "push_front should succeed on an empty list");
    ck_assert_msg(l->size == 1, "push_front should increment size");
    ck_assert_msg(l->head, "push_front should should add a node to list");
    ck_assert_msg(!l->head->next, "push_front should only add one node");
    ck_assert_msg(_user_equal(l->head, user), "push_front adds wrong data (double-check your create methods as well!)");

    _free_user(user);
    _free_list(l);
}
END_TEST

START_TEST(test_list_push_front_empty_instructor)
{
    struct user_list *l = _create_list();

    char *name = "Teetu";
    struct user *user = _create_user(name, INSTRUCTOR, (union user_data) {.instructor = {0.001}});

    ck_assert_msg(push_front(l, name, INSTRUCTOR, (union user_data) {.instructor = {0.001}}) == SUCCESS, "push_front should succeed on an empty list");
    ck_assert_msg(l->size == 1, "push_front should increment size");
    ck_assert_msg(l->head, "push_front should should add a node to list");
    ck_assert_msg(!l->head->next, "push_front should only add one node");
    ck_assert_msg(_user_equal(l->head, user), "push_front adds wrong data (double-check your create methods as well!)");

    _free_user(user);
    _free_list(l);
}
END_TEST

START_TEST(test_list_push_front_nonempty_student)
{
    struct user_list *l = _create_longer_list();

    char *name = "Naiviv";
    struct user *user = _create_user(name, STUDENT, (union user_data) {.student = {1, (double[]){34.2}}});

    ck_assert_msg(push_front(l, name, STUDENT, (union user_data) {.student = {1, (double[]){34.2}}}) == SUCCESS, "push_front should return SUCCESS on success");
    ck_assert_msg(l->size == 5, "push_front should increment size");
    ck_assert_msg(l->head, "push_front should should add a node to list");
    ck_assert_msg(_user_equal(l->head, user), "push_front adds wrong data (double-check your create methods as well!)");

    _free_user(user);
    _free_list(l);
}
END_TEST

START_TEST(test_list_push_front_nonempty_instructor)
{
    struct user_list *l = _create_longer_list();

    char *name = "Naiviv";
    struct user *user = _create_user(name, INSTRUCTOR, (union user_data) {.instructor = {1001.2}});

    ck_assert_msg(push_front(l, name, INSTRUCTOR, (union user_data) {.instructor = {1001.2}}) == SUCCESS, "push_front should return SUCCESS on success");
    ck_assert_msg(l->size == 5, "push_front should increment size");
    ck_assert_msg(l->head, "push_front should should add a node to list");
    ck_assert_msg(_user_equal(l->head, user), "push_front adds wrong data (double-check your create methods as well!)");

    _free_user(user);
    _free_list(l);
}
END_TEST

/******************************************************************************/
/************************** push_back() tests *********************************/
/******************************************************************************/
START_TEST(test_list_push_back_empty_student)
{
    struct user_list *l = _create_list();

    char *name = "Ailuj";
    struct user *user = _create_user(name, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}});

    ck_assert_msg(push_back(l, name, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}) == SUCCESS, "push_back should succeed on an empty list");
    ck_assert(l->head);
    ck_assert_msg(_user_equal(l->head, user), "push_back should add a node with correct data (double-check your create methods as well!)");
    ck_assert_msg(l->size == 1, "push_back should increment size");

    _free_user(user);
    _free_list(l);
}
END_TEST

START_TEST(test_list_push_back_empty_instructor)
{
    struct user_list *l = _create_list();

    char *name = "Ailuj";
    struct user *user = _create_user(name, INSTRUCTOR, (union user_data) {.instructor = {0.001}});

    ck_assert_msg(push_back(l, name, INSTRUCTOR, (union user_data) {.instructor = {0.001}}) == SUCCESS, "push_back should succeed on an empty list");
    ck_assert(l->head);
    ck_assert_msg(_user_equal(l->head, user), "push_back should add a node with correct data (double-check your create methods as well!)");
    ck_assert_msg(l->size == 1, "push_back should increment size");

    _free_user(user);
    _free_list(l);
}
END_TEST

START_TEST(test_list_push_back_malloc_failure)
{
    struct user_list *l = _create_longer_list();
    mallocs_until_fail = 0;

    int result = push_front(l, "Reemas", STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}});
    ck_assert_msg(result == FAILURE, "push_back should return FAILURE on malloc failure");

    _free_list(l);
}
END_TEST

START_TEST(test_list_push_back_non_empty)
{
    struct user_list *l = _create_list();
    ck_assert(l);

    struct user *user1 = _create_user("Ahos", STUDENT, (union user_data) {.student = {1, (double[]){34.2}}});
    _push_front(l, user1);

    char *name = "Etnoc";
    struct user *user2 = _create_user(name, INSTRUCTOR, (union user_data) {.instructor = {3349.8}});

    ck_assert_msg(push_back(l, name, INSTRUCTOR, (union user_data) {.instructor = {3349.8}}) == SUCCESS, "push_back should return SUCCESS on success");
    ck_assert_msg(l->head, "push_back should not change front of list");
    ck_assert_msg(_user_equal(l->head, user1), "push_back should not change front of list");
    ck_assert_msg(_user_equal(l->head->next, user2), "push_back should add a node with correct data (double-check your create methods as well!)");
    ck_assert_msg(l->size == 2, "push_back should increment size");

    _free_user(user2);
    _free_list(l); // will free user1
}
END_TEST

START_TEST(test_list_push_back_NULL)
{
    struct user_list *l = _create_list();

    char *name = "Akinrav";
    struct user *user = _create_user(NULL, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}});

    ck_assert_msg(push_back(NULL, name, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}) != SUCCESS, "push_back shouldn't return SUCCESS with NULL list");
    ck_assert_msg(push_back(l, NULL, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}) == SUCCESS, "push_back should return SUCCESS with NULL data and non-NULL list");
    ck_assert_msg(l->size == 1, "push_back should increase size");
    ck_assert_msg(_user_equal(l->head, user), "adding NULL name should add struct student with NULL name (double-check your create methods as well!)");

    _free_user(user);
    _free_list(l);
}
END_TEST

/******************************************************************************/
/**************************** remove_at_index() tests *************************/
/******************************************************************************/
START_TEST(test_list_remove_at_index_NULL_list)
{
    struct user *dataOut = (struct user *)0xDEADBEEFU;
    ck_assert_msg(remove_at_index(NULL, &dataOut, 0) != SUCCESS, "remove_at_index shouldn't return SUCCESS with NULL list");
}
END_TEST

START_TEST(test_list_remove_at_index_NULL_dataOut)
{
    struct user_list *l = _create_length_1_list();
    ck_assert_msg(remove_at_index(l, NULL, 0) != SUCCESS, "remove_at_index shouldn't return SUCCESS with NULL dataOut");
    _free_list(l);
}
END_TEST

START_TEST(test_list_remove_at_index_out_of_bounds)
{
    struct user_list *l = _create_length_1_list();
    ck_assert_msg(remove_at_index(l, NULL, -1) != SUCCESS, "remove_at_index shouldn't return SUCCESS when out of bounds");
    _free_list(l);
}
END_TEST

START_TEST(test_list_remove_at_index_front)
{
    struct user_list *l = _create_longer_list();
    struct user *dataOut = NULL;
    int result = remove_at_index(l, &dataOut, 0);
    ck_assert_msg(dataOut, "remove_at_index does not set the value of dataOut");
    int userCheck = _user_equal(dataOut, &sample_users[0]);
    char tempbuf[1000];
    sprintf(tempbuf, "%s %s", dataOut->name, sample_users[0].name);
    _free_user(dataOut);
    ck_assert_msg(result == SUCCESS, "remove_at_index should return SUCCESS when successful");
    ck_assert_msg(userCheck, tempbuf);
    ck_assert_int_eq(l->size, 3);

    ck_assert(l->head);
    ck_assert(_user_equal(l->head, &sample_users[1]));

    ck_assert(l->head->next);
    ck_assert(_user_equal(l->head->next, &sample_users[2]));

    _free_list(l);
}
END_TEST

START_TEST(test_list_remove_at_index_middle)
{
    struct user_list *l = _create_longer_list();
    struct user *dataOut = NULL;
    int result = remove_at_index(l, &dataOut, 1);
    ck_assert_msg(dataOut, "remove_at_index does not set the value of dataOut");
    int userCheck = _user_equal(dataOut, &sample_users[1]);
    _free_user(dataOut);
    ck_assert_msg(result == SUCCESS, "remove_at_index should return SUCCESS when successful");
    ck_assert_msg(userCheck, "remove_at_index sets incorrect value for dataOut");
    ck_assert_int_eq(l->size, 3);

    ck_assert(l->head);
    ck_assert(_user_equal(l->head, &sample_users[0]));

    ck_assert(l->head->next);
    ck_assert(_user_equal(l->head->next, &sample_users[2]));

    ck_assert(l->head->next->next);
    ck_assert(_user_equal(l->head->next->next, &sample_users[3]));

    ck_assert(!l->head->next->next->next);

    _free_list(l);
}
END_TEST

START_TEST(test_list_remove_at_index_back)
{
    struct user_list *l = _create_longer_list();
    struct user *dataOut = NULL;
    int result = remove_at_index(l, &dataOut, 3);
    ck_assert_msg(dataOut, "remove_at_index does not set the value of dataOut");
    int userCheck = _user_equal(dataOut, &sample_users[3]);
    _free_user(dataOut);
    ck_assert_msg(result == SUCCESS, "remove_at_index should return SUCCESS when successful");
    ck_assert_msg(userCheck, "remove_at_index sets incorrect value for dataOut");
    ck_assert_int_eq(l->size, 3);

    ck_assert(l->head);
    ck_assert(_user_equal(l->head, &sample_users[0]));

    ck_assert(l->head->next);
    ck_assert(_user_equal(l->head->next, &sample_users[1]));

    ck_assert(l->head->next->next);
    ck_assert(_user_equal(l->head->next->next, &sample_users[2]));

    ck_assert(!l->head->next->next->next);

    _free_list(l);
}
END_TEST

START_TEST(test_list_remove_at_index_length_1)
{
    struct user_list *l = _create_length_1_list();
    struct user *dataOut = NULL;
    int result = remove_at_index(l, &dataOut, 0);
    ck_assert_msg(dataOut, "remove_at_index does not set the value of dataOut");
    int userCheck = _user_equal(dataOut, &sample_user_student);
    _free_user(dataOut);
    ck_assert_msg(result == SUCCESS, "remove_at_index should return SUCCESS when successful");
    ck_assert_msg(userCheck, "remove_at_index sets incorrect value for dataOut");
    ck_assert_int_eq(l->size, 0);
    ck_assert(!l->head);

    _free_list(l);
}
END_TEST

/******************************************************************************/
/************************** add_at_index() tests ***************************************/
/******************************************************************************/
START_TEST(test_list_add_at_index_out_of_bounds)
{
    struct user_list *l = _create_list();

    char *name = "Wehttam";

    ck_assert_msg(add_at_index(l, -1, name, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}) != SUCCESS, "add_at_index shouldn't return SUCCESS for index < 0");
    _push_front(l, _create_user(name, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}));
    ck_assert_msg(add_at_index(l, 2, name, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}) != SUCCESS, "add_at_index shouldn't return SUCCESS for index >= size");

    // free_list will free the rest of the data since we manually inserted it at the head
    _free_list(l);
}
END_TEST

START_TEST(test_list_add_at_index_empty_list)
{
    struct user_list *l = _create_list();

    char *name1 = "Kcirtap";
    struct user *user1 = _create_user(name1, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}});

    char *name2 = "Tsugua";

    ck_assert(add_at_index(l, 0, name1, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}) == SUCCESS);
    ck_assert_int_eq(l->size, 1);

    ck_assert(l->head);
    ck_assert(_user_equal(l->head, user1));

    ck_assert_msg(add_at_index(l, 2, name2, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}) != SUCCESS, "add_at_index shouldn't return SUCCESS for index >= size");
    ck_assert_msg(add_at_index(l, 1, name2, STUDENT, (union user_data) {.student = {2, (double[]){43.9, 20.1}}}) == SUCCESS, "add_at_index should return SUCCESS when successful");

    ck_assert_int_eq(l->size, 2);

    _free_user(user1);
    _free_list(l);
}
END_TEST

START_TEST(test_list_add_at_index_non_empty_list)
{
    struct user_list *l = _create_longer_list();

    char *name = "Ellebasi";
    struct user *user = _create_user(name, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}});

    ck_assert_msg(add_at_index(l, 1, name, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}) == SUCCESS, "add_at_index should return SUCCESS when successful");

    ck_assert(l->head);
    ck_assert(l->head->next);
    ck_assert(_user_equal(l->head->next, user));

    ck_assert_int_eq(l->size, 5);

    _free_user(user);
    _free_list(l);
}
END_TEST

START_TEST(test_list_add_at_index_NULL)
{
    struct user_list *l = _create_list();

    struct user *user = _create_user(NULL, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}});

    ck_assert_msg(add_at_index(NULL, 0, NULL, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}) != SUCCESS, "add_at_index shouldn't return SUCCESS for NULL list");
    ck_assert_msg(add_at_index(NULL, 0, "Yeroc", STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}) != SUCCESS, "add_at_index shouldn't return SUCCESS for NULL list");

    ck_assert_msg(add_at_index(l, 0, NULL, STUDENT, (union user_data) {.student = {3, (double[]){59.9, 60.1, 100}}}) == SUCCESS, "add_at_index shouldn't fail with NULL name");
    ck_assert_int_eq(l->size, 1);

    ck_assert(l->head);
    ck_assert(_user_equal(l->head, user));

    _free_user(user);
    _free_list(l);
}
END_TEST

/******************************************************************************/
/*************************** pop_front() tests ********************************/
/******************************************************************************/
START_TEST(test_list_pop_front_NULL_list)
{
    struct user *dataOut = (struct user *)0xDEADBEEFU;
    ck_assert_msg(pop_front(NULL, &dataOut) != SUCCESS, "pop_front shouldn't return SUCCESS with NULL list");
}
END_TEST

START_TEST(test_list_pop_front_NULL_dataOut)
{
    struct user_list *l = _create_list();
    ck_assert_msg(pop_front(l, NULL) != SUCCESS, "pop_front shouldn't return SUCCESS with NULL dataOut");
    _free_list(l);
}
END_TEST

START_TEST(test_list_pop_front_empty)
{
    struct user_list *l = _create_list();
    struct user *dataOut = (struct user *)0xDEADBEEFU;
    ck_assert_msg(pop_front(l, &dataOut) != SUCCESS, "pop_front shouldn't return SUCCESS when called with empty list");
    _free_list(l);
}
END_TEST

START_TEST(test_list_pop_front_nonempty)
{
    struct user_list *l = _create_length_1_list();
    struct user *user = NULL;
    int result = pop_front(l, &user);
    ck_assert_msg(user, "pop_front should set dataOut on success");
    int userCheck = _user_equal(user, &sample_user_student);
    _free_user(user);
    ck_assert_msg(result == SUCCESS, "pop_front should return SUCCESS on success");
    ck_assert_msg(userCheck, "removed node has incorrect value");
    ck_assert_int_eq(l->size, 0);

    _free_list(l);
}
END_TEST

START_TEST(test_list_pop_front_longer_list)
{
    struct user_list *l = _create_longer_list();
    struct user *data = NULL;
    int result = pop_front(l, &data);
    ck_assert_msg(data, "pop_front should set dataOut on success");
    int userCheck = _user_equal(data, &sample_users[0]);
    _free_user(data);
    ck_assert_msg(result == SUCCESS, "pop_front should return SUCCESS on success");
    ck_assert_msg(userCheck, "removed node has incorrect value");
    ck_assert_int_eq(l->size, 3);

    ck_assert(l->head);
    ck_assert(_user_equal(l->head, &sample_users[1]));

    ck_assert(l->head->next);
    ck_assert(_user_equal(l->head->next, &sample_users[2]));

    ck_assert(l->head->next->next);
    ck_assert(_user_equal(l->head->next->next, &sample_users[3]));

    ck_assert(!l->head->next->next->next);

    result = pop_front(l, &data);
    ck_assert(data);
    userCheck = _user_equal(data, &sample_users[1]);
    _free_user(data);

    ck_assert_msg(result == SUCCESS, "pop_front should return SUCCESS on success");
    ck_assert_msg(userCheck, "removed node has incorrect value");
    ck_assert_int_eq(l->size, 2);

    ck_assert(l->head);
    ck_assert(_user_equal(l->head, &sample_users[2]));

    ck_assert(l->head->next);
    ck_assert(_user_equal(l->head->next, &sample_users[3]));

    ck_assert(!l->head->next->next);

    _free_list(l);
}
END_TEST

/******************************************************************************/
/**************************** pop_back() tests ********************************/
/******************************************************************************/
START_TEST(test_list_pop_back_NULL_list)
{
    struct user *dataOut = (struct user *)0xDEADBEEFU;
    ck_assert_msg(pop_back(NULL, &dataOut) != SUCCESS, "pop_back shouldn't return SUCCESS with NULL list");
}
END_TEST

START_TEST(test_list_pop_back_NULL_dataOut)
{
    struct user_list *l = _create_list();
    ck_assert_msg(pop_back(l, NULL) != SUCCESS, "pop_back shouldn't return SUCCESS with NULL dataOut");
    _free_list(l);
}
END_TEST

START_TEST(test_list_pop_back_empty)
{
    struct user_list *l = _create_list();
    struct user *dataOut = (struct user *)0xDEADBEEFU;
    ck_assert_msg(pop_back(l, &dataOut) != SUCCESS, "pop_back shouldn't return SUCCESS when called with empty list");
    _free_list(l);
}
END_TEST

START_TEST(test_list_pop_back_nonempty)
{
    struct user_list *l = _create_length_1_list();
    struct user *user = NULL;
    int result = pop_back(l, &user);
    ck_assert_msg(user, "pop_back should set dataOut on success");

    int userCheck = _user_equal(user, &sample_user_student);
    _free_user(user);

    ck_assert_msg(result == SUCCESS, "pop_back should return SUCCESS on success");
    ck_assert_msg(userCheck, "removed node has incorrect value");
    ck_assert_int_eq(l->size, 0);

    _free_list(l);
}
END_TEST

START_TEST(test_list_pop_back_longer_list)
{
    struct user_list *l = _create_longer_list();
    struct user *data = NULL;
    int result = pop_back(l, &data);
    ck_assert(data);

    int userCheck = _user_equal(data, &sample_users[3]);
    _free_user(data);

    ck_assert_msg(result == SUCCESS, "pop_back should return SUCCESS on success");
    ck_assert_msg(userCheck, "removed node has incorrect value");
    ck_assert_int_eq(l->size, 3);

    ck_assert(l->head);
    ck_assert(_user_equal(l->head, &sample_users[0]));
    ck_assert(l->head->next);
    ck_assert(_user_equal(l->head->next, &sample_users[1]));
    ck_assert(l->head->next->next);
    ck_assert(_user_equal(l->head->next->next, &sample_users[2]));

    ck_assert(!l->head->next->next->next);

    result = pop_back(l, &data);
    userCheck = _user_equal(data, &sample_users[2]);
    _free_user(data);

    ck_assert_msg(result == SUCCESS, "pop_back should return SUCCESS on success");
    ck_assert_msg(userCheck, "removed node has incorrect value");
    ck_assert_int_eq(l->size, 2);

    ck_assert(l->head);
    ck_assert(_user_equal(l->head, &sample_users[0]));

    ck_assert(l->head->next);
    ck_assert(_user_equal(l->head->next, &sample_users[1]));

    ck_assert(!l->head->next->next);

    _free_list(l);
}
END_TEST

/******************************************************************************/
/******************************* get() tests **********************************/
/******************************************************************************/
// helper method for get() tests
void checkLongerListUnchanged(struct user_list *l, char *msg)
{
    ck_assert_msg(_user_equal(l->head, &sample_users[0]), msg);
    ck_assert_msg(_user_equal(l->head->next, &sample_users[1]), msg);
    ck_assert_msg(_user_equal(l->head->next->next, &sample_users[2]), msg);
    ck_assert_msg(_user_equal(l->head->next->next->next, &sample_users[3]), msg);
    ck_assert_msg(!l->head->next->next->next->next, msg);
}

START_TEST(test_list_get_NULL_list)
{
    struct user *u = (struct user *)0xDEADBEEFU;
    ck_assert_msg(get(NULL, 0, &u) != SUCCESS, "get shouldn't return SUCCESS when called with NULL list");
}
END_TEST

START_TEST(test_list_get_NULL_dataOut)
{
    struct user_list *l = _create_longer_list();
    ck_assert_msg(get(l, 0, NULL) != SUCCESS, "get shouldn't return SUCCESS when called with NULL dataOut");
    checkLongerListUnchanged(l, "list was changed by get method");
    _free_list(l);
}
END_TEST

START_TEST(test_list_get_negative_index)
{
    struct user_list *l = _create_longer_list();
    struct user *u = (struct user *)0xDEADBEEFU;
    ck_assert_msg(get(l, -1, &u) != SUCCESS, "get shouldn't return SUCCESS when called with index < 0");
    checkLongerListUnchanged(l, "list was changed by get method");
    _free_list(l);
}
END_TEST

START_TEST(test_list_get_front)
{
    struct user_list *l = _create_longer_list();
    struct user *u = (struct user *)0xDEADBEEFU;
    ck_assert_msg(get(l, 0, &u) == SUCCESS, "get should return SUCCESS on success");
    ck_assert_msg(_user_equal(u, &sample_users[0]), "get assigns incorrect result to dataOut");
    checkLongerListUnchanged(l, "list was changed by get method");
    _free_list(l);
}
END_TEST

START_TEST(test_list_get_middle)
{
    struct user_list *l = _create_longer_list();
    struct user *u = (struct user *)0xDEADBEEFU;
    ck_assert_msg(get(l, 1, &u) == SUCCESS, "get should return SUCCESS on success");
    ck_assert_msg(_user_equal(u, &sample_users[1]), "get assigns incorrect result to dataOut");
    checkLongerListUnchanged(l, "list was changed by get method");
    _free_list(l);
}
END_TEST

START_TEST(test_list_get_back)
{
    struct user_list *l = _create_longer_list();
    struct user *u = (struct user *)0xDEADBEEFU;
    ck_assert_msg(get(l, 2, &u) == SUCCESS, "get should return SUCCESS on success");
    ck_assert_msg(_user_equal(u, &sample_users[2]), "get assigns incorrect result to dataOut");
    checkLongerListUnchanged(l, "list was changed by get method");
    _free_list(l);
}
END_TEST

START_TEST(test_list_get_out_of_bounds_index)
{
    struct user_list *l = _create_longer_list();
    struct user *u = (struct user *)0xDEADBEEFU;
    ck_assert_msg(get(l, 4, &u) != SUCCESS, "get shouldn't return SUCCESS when called with index >= size");
    checkLongerListUnchanged(l, "list was changed by get method");
    _free_list(l);
}
END_TEST

/******************************************************************************/
/**************************** contains() tests ********************************/
/******************************************************************************/

START_TEST(test_list_contains_NULL_list)
{
    struct user *user = _create_user("Draheraseman", STUDENT, (union user_data) {.student = {1, (double[]){34.2}}});

    struct user *dataOut = (struct user *)0xDEADBEEFU;
    int result = contains(NULL, user, &dataOut);

    _free_user(user);
    ck_assert_msg(result == FAILURE, "contains should return FAILURE when called with NULL list");
}
END_TEST

START_TEST(test_list_contains_NULL_dataOut)
{
    struct user_list *l = _create_longer_list();

    // this is a terrible joke
    struct user *user = _create_user("Dr. LC-3, Human-Cyborg Relations", STUDENT, (union user_data) {.student = {1, (double[]){34.2}}});

    int result = contains(l, user, NULL);

    _free_user(user);
    ck_assert_msg(result == FAILURE, "contains should return FAILURE when called with NULL dataOut");
    checkLongerListUnchanged(l, "list was changed by contains method");

    _free_list(l);
}
END_TEST

START_TEST(test_list_contains_empty)
{
    struct user_list *l = _create_list();

    struct user *user = _create_user("Dr. Seuss", STUDENT, (union user_data) {.student = {1, (double[]){34.2}}});

    struct user *dataOut = (struct user *)0xDEADBEEFU;
    int result = contains(l, user, &dataOut);

    _free_user(user);
    ck_assert_msg(result == FAILURE, "contains should return FAILURE when data not found");
    ck_assert_msg(!dataOut, "contains should set dataOut=NULL when data not found");

    _free_list(l);
}
END_TEST

START_TEST(test_list_contains_not_contained)
{
    struct user_list *l = _create_longer_list();

    struct user *user = _create_user("Mr. Unknown", STUDENT, (union user_data) {.student = {1, (double[]){34.2}}});

    struct user *dataOut = (struct user *)0xDEADBEEFU;
    int result = contains(l, user, &dataOut);

    _free_user(user);
    ck_assert_msg(result == FAILURE, "contains should return FAILURE when data not found");
    ck_assert_msg(!dataOut, "contains should set dataOut=NULL when data not found");
    checkLongerListUnchanged(l, "list was changed by contains method");

    _free_list(l);
}
END_TEST

START_TEST(test_list_contains_NULL_name)
{
    struct user_list *l = _create_longer_list();

    struct user *user = _create_user(NULL, STUDENT, (union user_data) {.student = {1, (double[]){34.2}}});

    _push_front(l, _create_user(NULL, STUDENT, (union user_data) {.student = {1, (double[]){34.2}}}));

    struct user *dataOut = (struct user *)0xDEADBEEFU;
    int result = contains(l, user, &dataOut);

    ck_assert_msg(result == SUCCESS, "contains should return SUCCESS when data is found");
    ck_assert_msg(dataOut != user, "contains should set dataOut to the value in the list, not the one passed in");
    ck_assert_msg(_user_equal(dataOut, user), "contains should set dataOut to the value in the list that matches the value passed in");

    dataOut = _pop_front(l);
    _free_user(dataOut);
    _free_user(user);

    checkLongerListUnchanged(l, "list was changed by contains method");
    _free_list(l);
}
END_TEST

START_TEST(test_list_contains_is_contained)
{
    struct user_list *l = _create_longer_list();

    struct user *dataOut = (struct user *)0xDEADBEEFU;
    int result = contains(l, &sample_users[1], &dataOut);

    ck_assert_msg(result == SUCCESS, "contains should return SUCCESS when data is found");
    ck_assert_msg(dataOut != &sample_users[1], "contains should set dataOut to the value in the list, not the one passed in");
    ck_assert_msg(_user_equal(dataOut, &sample_users[1]), "contains should set dataOut to the value in the list that matches the value passed in");

    checkLongerListUnchanged(l, "list was changed by contains method");
    _free_list(l);
}
END_TEST

/******************************************************************************/
/************************** empty_list() tests ********************************/
/******************************************************************************/

/* NOTE: passing these tests does not mean that there are no memory leaks!
This is an easy method to accidentally cause memory leaks on, so be sure to run Valgrind! */

START_TEST(test_list_empty_list_NULL_list)
{
    // Success if there is no error
    empty_list(NULL);
}
END_TEST

START_TEST(test_list_empty_list_empty)
{
    struct user_list *l = _create_list();
    empty_list(l);
    ck_assert_int_eq(l->size, 0);
    ck_assert_msg(!l->head, "head should point to NULL after empty_list");
    _free_list(l);
}
END_TEST

START_TEST(test_list_empty_list_length_1)
{
    struct user_list *l = _create_length_1_list();
    empty_list(l);
    ck_assert_int_eq(l->size, 0);
    ck_assert_msg(!l->head, "head should point to NULL after empty_list");
    _free_list(l);
}
END_TEST

START_TEST(test_list_empty_list_longer_list)
{
    struct user_list *l = _create_longer_list();
    empty_list(l);
    ck_assert_int_eq(l->size, 0);
    ck_assert_msg(!l->head, "head should point to NULL after empty_list");
    _free_list(l);
}
END_TEST

/******************************************************************************/
/************************** get_highest_paid() tests **************************/
/******************************************************************************/

START_TEST(test_list_get_highest_paid_null_list)
{
    struct user *dataOut = (struct user *)0xDEADBEEFU;
    int result = get_highest_paid(NULL, &dataOut);
    ck_assert_msg(dataOut == NULL, "get_highest_paid does not set the value of dataOut to NULL");
    ck_assert_msg(result == FAILURE, "get_highest_paid should return FAILURE when list is NULL");
}
END_TEST

START_TEST(test_list_get_highest_paid_empty_list)
{
    struct user_list *l = _create_list();
    struct user *dataOut = (struct user *)0xDEADBEEFU;
    int result = get_highest_paid(l, &dataOut);
    ck_assert_msg(dataOut == NULL, "get_highest_paid does not set the value of dataOut to NULL");
    ck_assert_msg(result == FAILURE, "get_highest_paid should return FAILURE when list is empty");

    struct user_list *original = _create_list();
    checkListEquals(l, original, "list was changed by get_highest_paid method");

    _free_list(l);
    _free_list(original);
}
END_TEST

START_TEST(test_list_get_highest_paid_no_instructors)
{
    struct user_list *l = _create_list();
    // Push in reverse order
    _push_front(l, _create_user("Timud", STUDENT, (union user_data) {.student = {1, (double[]){55.3}}}));
    _push_front(l, _create_user("Nrehtous", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    struct user *dataOut = (struct user *)0xDEADBEEFU;
    int result = get_highest_paid(l, &dataOut);
    ck_assert_msg(dataOut == NULL, "get_highest_paid does not set the value of dataOut to NULL");
    ck_assert_msg(result == SUCCESS, "get_highest_paid should return SUCCESS when successful");

    struct user_list *original = _create_list();
    // Push in reverse order
    _push_front(original, _create_user("Timud", STUDENT, (union user_data) {.student = {1, (double[]){55.3}}}));
    _push_front(original, _create_user("Nrehtous", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    checkListEquals(l, original, "list was changed by get_highest_paid method");

    _free_list(l);
    _free_list(original);
}
END_TEST

START_TEST(test_list_get_highest_paid_multiple_highest)
{
    struct user_list *l = _create_list();
    struct user highest_paid = {NULL, "Reemas", INSTRUCTOR, (union user_data) {.instructor = {10000.5}}};
    // Push in reverse order
    _push_front(l, _create_user("Timud", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    _push_front(l, _create_user("Nrehtous", INSTRUCTOR, (union user_data) {.instructor = {10000.5}}));
    _push_front(l, _create_user("Reemas", INSTRUCTOR, (union user_data) {.instructor = {10000.5}}));
    _push_front(l, _create_user("Balec", INSTRUCTOR, (union user_data) {.instructor = {5555.9}}));
    struct user *dataOut = NULL;
    int result = get_highest_paid(l, &dataOut);
    ck_assert_msg(dataOut, "get_highest_paid does not set the value of dataOut");
    ck_assert_msg(result == SUCCESS, "get_highest_paid should return SUCCESS when successful");
    ck_assert_msg(_user_equal(dataOut, &highest_paid), "get_highest_paid sets incorrect value for dataOut");

    struct user_list *original = _create_list();
    // Push in reverse order
    _push_front(original, _create_user("Timud", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    _push_front(original, _create_user("Nrehtous", INSTRUCTOR, (union user_data) {.instructor = {10000.5}}));
    _push_front(original, _create_user("Reemas", INSTRUCTOR, (union user_data) {.instructor = {10000.5}}));
    _push_front(original, _create_user("Balec", INSTRUCTOR, (union user_data) {.instructor = {5555.9}}));
    checkListEquals(l, original, "list was changed by get_highest_paid method");

    _free_list(l);
    _free_list(original);
}
END_TEST

START_TEST(test_list_get_highest_paid_normal)
{
    struct user_list *l = _create_longer_list();
    struct user *dataOut = NULL;
    int result = get_highest_paid(l, &dataOut);
    ck_assert_msg(dataOut, "get_highest_paid does not set the value of dataOut");
    ck_assert_msg(result == SUCCESS, "get_highest_paid should return SUCCESS when successful");
    ck_assert_msg(_user_equal(dataOut, &sample_users[3]), "get_highest_paid sets incorrect value for dataOut");

    checkLongerListUnchanged(l, "list was changed by get_highest_paid method");

    _free_list(l);
}
END_TEST

/******************************************************************************/
/************************** is_passing_all_classes() tests ********************/
/******************************************************************************/

START_TEST(test_list_is_passing_all_classes_empty_list)
{
    struct user_list *l = _create_list();
    int dataOut = -2;
    int result = is_passing_all_classes(l, "Reemas", &dataOut);
    ck_assert_msg(dataOut == -1, "is_passing_all_classes does not correctly set the value of dataOut");
    ck_assert_msg(result == FAILURE, "is_passing_all_classes should return SUCCESS when successful");

    _free_list(l);
}
END_TEST

START_TEST(test_list_is_passing_all_classes_null_list)
{
    int dataOut = -2;
    int result = is_passing_all_classes(NULL, "Reemas", &dataOut);
    ck_assert_msg(dataOut == -1, "is_passing_all_classes does not correctly set the value of dataOut");
    ck_assert_msg(result == FAILURE, "is_passing_all_classes should return SUCCESS when successful");
}
END_TEST

START_TEST(test_list_is_passing_all_classes_student_not_contained)
{
    struct user_list *l = _create_list();
    // Push in reverse order
    _push_front(l, _create_user("Timud", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    _push_front(l, _create_user("Nrehtous", INSTRUCTOR, (union user_data) {.instructor = {10000.5}}));
    _push_front(l, _create_user("Reemas", STUDENT, (union user_data) {.student = {3, (double[]){99.9, 50.6, 85.7}}}));
    _push_front(l, _create_user("Balec", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    int dataOut = -2;
    int result = is_passing_all_classes(l, "Mario", &dataOut);
    ck_assert_msg(dataOut == -1, "is_passing_all_classes does not correctly set the value of dataOut");
    ck_assert_msg(result == FAILURE, "is_passing_all_classes should return SUCCESS when successful");

    _free_list(l);
}
END_TEST

START_TEST(test_list_is_passing_all_classes_no_classes)
{
    struct user_list *l = _create_list();
    // Push in reverse order
    _push_front(l, _create_user("Timud", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    _push_front(l, _create_user("Nrehtous", INSTRUCTOR, (union user_data) {.instructor = {10000.5}}));
    union user_data reemas;
    double *empty_arr = malloc(0);
    reemas.student.grades = empty_arr;
    reemas.student.num_classes = 0;
    _push_front(l, _create_user("Reemas", STUDENT, reemas));
    _push_front(l, _create_user("Balec", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    int dataOut = -2;
    int result = is_passing_all_classes(l, "Reemas", &dataOut);
    ck_assert_msg(dataOut == 0, "is_passing_all_classes does not correctly set the value of dataOut");
    ck_assert_msg(result == SUCCESS, "is_passing_all_classes should return SUCCESS when successful");

    _free_list(l);
    free(empty_arr);
}
END_TEST

START_TEST(test_list_is_passing_all_classes_same_name_instructor)
{
    struct user_list *l = _create_list();
    // Push in reverse order
    _push_front(l, _create_user("Timud", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    _push_front(l, _create_user("Reemas", INSTRUCTOR, (union user_data) {.instructor = {10000.5}}));
    _push_front(l, _create_user("Reemas", STUDENT, (union user_data) {.student = {3, (double[]){99.9, 65.6, 85.7}}}));
    _push_front(l, _create_user("Balec", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    int dataOut = -2;
    int result = is_passing_all_classes(l, "Reemas", &dataOut);
    ck_assert_msg(dataOut == 1, "is_passing_all_classes does not correctly set the value of dataOut");
    ck_assert_msg(result == SUCCESS, "is_passing_all_classes should return SUCCESS when successful");

    _free_list(l);
}
END_TEST

START_TEST(test_list_is_passing_all_classes_passing)
{
    struct user_list *l = _create_list();
    // Push in reverse order
    _push_front(l, _create_user("Timud", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    _push_front(l, _create_user("Nrehtous", INSTRUCTOR, (union user_data) {.instructor = {10000.5}}));
    _push_front(l, _create_user("Balec", STUDENT, (union user_data) {.student = {3, (double[]){60.0, 65.3, 100.0}}}));
    _push_front(l, _create_user("Reemas", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    int dataOut = -2;
    int result = is_passing_all_classes(l, "Balec", &dataOut);
    ck_assert_msg(dataOut == 1, "is_passing_all_classes does not correctly set the value of dataOut");
    ck_assert_msg(result == SUCCESS, "is_passing_all_classes should return SUCCESS when successful");

    _free_list(l);
}
END_TEST

START_TEST(test_list_is_passing_all_classes_failing_all)
{
    struct user_list *l = _create_list();
    // Push in reverse order
    _push_front(l, _create_user("Timud", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    _push_front(l, _create_user("Nrehtous", INSTRUCTOR, (union user_data) {.instructor = {10000.5}}));
    _push_front(l, _create_user("Balec", STUDENT, (union user_data) {.student = {3, (double[]){59.9, 32.3, 40.0}}}));
    _push_front(l, _create_user("Reemas", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    int dataOut = -2;
    int result = is_passing_all_classes(l, "Balec", &dataOut);
    ck_assert_msg(dataOut == 0, "is_passing_all_classes does not correctly set the value of dataOut");
    ck_assert_msg(result == SUCCESS, "is_passing_all_classes should return SUCCESS when successful");

    _free_list(l);
}
END_TEST

START_TEST(test_list_is_passing_all_classes_failing_some)
{
    struct user_list *l = _create_list();
    // Push in reverse order
    _push_front(l, _create_user("Timud", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    _push_front(l, _create_user("Nrehtous", INSTRUCTOR, (union user_data) {.instructor = {10000.5}}));
    _push_front(l, _create_user("Reemas", STUDENT, (union user_data) {.student = {3, (double[]){99.9, 50.6, 85.7}}}));
    _push_front(l, _create_user("Balec", STUDENT, (union user_data) {.student = {3, (double[]){95.2, 32.1, 54.5}}}));
    int dataOut = -2;
    int result = is_passing_all_classes(l, "Reemas", &dataOut);
    ck_assert_msg(dataOut == 0, "is_passing_all_classes does not correctly set the value of dataOut");
    ck_assert_msg(result == SUCCESS, "is_passing_all_classes should return SUCCESS when successful");

    _free_list(l);
}
END_TEST

/******************************************************************************/
/************************** end_semester() tests ******************************/
/******************************************************************************/

START_TEST(test_list_end_semester_null_list)
{
    int result = end_semester(NULL);
    ck_assert_msg(result == FAILURE, "end_semester should return FAILURE when list is NULL");
}
END_TEST

START_TEST(test_list_end_semester_empty_list)
{
    struct user_list *l = _create_list();
    int result = end_semester(l);
    ck_assert_msg(result == FAILURE, "end_semester should return FAILURE when list is NULL");
    ck_assert_msg(l->head == NULL, "end_semester should not change an empty list");

    _free_list(l);
}
END_TEST

START_TEST(test_list_end_semester_normal)
{
    struct user_list *l = _create_list();
    // Push in reverse order
    _push_front(l, _create_user("Timud", STUDENT, (union user_data) {.student = {1, (double[]){95.2}}}));
    _push_front(l, _create_user("Nrehtous", INSTRUCTOR, (union user_data) {.instructor = {25000.5}}));
    _push_front(l, _create_user("Reemas", STUDENT, (union user_data) {.student = {3, (double[]){99.9, 50.6, 85.7}}}));
    _push_front(l, _create_user("Balec", INSTRUCTOR, (union user_data) {.instructor = {2.5}}));
    int result = end_semester(l);
    ck_assert_msg(result == SUCCESS, "end_semester should return SUCCESS when successful");

    struct user_list *correct = _create_list();
    // Push in reverse order
    _push_front(correct, _create_user("Timud", STUDENT, (union user_data) {.student = {0, NULL}}));
    _push_front(correct, _create_user("Nrehtous", INSTRUCTOR, (union user_data) {.instructor = {35000.5}}));
    _push_front(correct, _create_user("Reemas", STUDENT, (union user_data) {.student = {0, NULL}}));
    _push_front(correct, _create_user("Balec", INSTRUCTOR, (union user_data) {.instructor = {10002.5}}));
    checkListEquals(l, correct, "end_semester does not correctly update the list");

    _free_list(l);
    _free_list(correct);
}
END_TEST

Suite *list_suite(void)
{
    Suite *s = suite_create("struct user_list");

    // create_list() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_create_list_basic);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_create_list_malloc_failure);

    // push_front() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_front_NULL_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_front_malloc_failure);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_front_NULL_name_student);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_front_NULL_name_instructor);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_front_empty_student);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_front_empty_instructor);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_front_nonempty_student);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_front_nonempty_instructor);

    // push_back() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_back_empty_student);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_back_empty_instructor);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_back_malloc_failure);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_back_non_empty);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_push_back_NULL);

    // remove_at_index() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_remove_at_index_NULL_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_remove_at_index_NULL_dataOut);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_remove_at_index_out_of_bounds);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_remove_at_index_front);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_remove_at_index_middle);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_remove_at_index_back);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_remove_at_index_length_1);

    // pop_front() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_pop_front_NULL_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_pop_front_NULL_dataOut);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_pop_front_empty);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_pop_front_nonempty);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_pop_front_longer_list);

    // pop_back() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_pop_back_NULL_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_pop_back_NULL_dataOut);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_pop_back_empty);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_pop_back_nonempty);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_pop_back_longer_list);

    // add_at_index() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_add_at_index_out_of_bounds);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_add_at_index_empty_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_add_at_index_non_empty_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_add_at_index_NULL);

    // get() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_get_NULL_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_get_NULL_dataOut);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_get_negative_index);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_get_front);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_get_middle);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_get_back);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_get_out_of_bounds_index);

    // contains() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_contains_NULL_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_contains_NULL_dataOut);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_contains_empty);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_contains_not_contained);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_contains_NULL_name);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_contains_is_contained);

    // empty_list() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_empty_list_NULL_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_empty_list_empty);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_empty_list_length_1);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_empty_list_longer_list);

    // get_highest_paid() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_get_highest_paid_null_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_get_highest_paid_empty_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_get_highest_paid_no_instructors);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_get_highest_paid_multiple_highest);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_get_highest_paid_normal);

    // is_passing_all_classes() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_is_passing_all_classes_empty_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_is_passing_all_classes_null_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_is_passing_all_classes_student_not_contained);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_is_passing_all_classes_no_classes);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_is_passing_all_classes_same_name_instructor);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_is_passing_all_classes_passing);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_is_passing_all_classes_failing_all);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_is_passing_all_classes_failing_some);
    

    // // end_semester() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_end_semester_null_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_end_semester_empty_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_end_semester_normal);

    return s;
}
