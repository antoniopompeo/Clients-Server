#include <stdlib.h>

#include "list.h"

static void list_link_element(t_link_element* previous, t_link_element* next);
static t_link_element* list_create_element(void* data);
static t_link_element* list_get_element(t_list* self, int index);
static t_link_element* list_find_element(t_list *self, bool(*condition)(void*), int* index);

/*
 * @NAME: list_create
 * @DESC: Crea una lista
 */

t_list *list_create() {
	t_list *list = malloc(sizeof(t_list));
	list->head = NULL;
	list->elements_count = 0;
	return list;
}

/*
 * @NAME: list_add
 * @DESC: Agrega un elemento al final de la lista
 */
int list_add(t_list *self, void *data) {
	t_link_element *new_element = list_create_element(data);

	if (self->elements_count == 0) {
		self->head = new_element;
	} else {
		list_link_element(list_get_element(self, self->elements_count - 1), new_element);
	}
	self->elements_count++;
	return self->elements_count - 1;
}

/*
 * @NAME: list_get
 * @DESC: Retorna el contenido de una posicion determianda de la lista
 */
void* list_get(t_list *self, int index) {
	t_link_element* element_in_index = list_get_element(self, index);
	return element_in_index != NULL ? element_in_index->data : NULL;
}

/*
 * @NAME: list_add_in_index
 * @DESC: Agrega un elemento en una posicion determinada de la lista
 */
void list_add_in_index(t_list *self, int index, void *data) {
	t_link_element* next = NULL;
	t_link_element* new_element = NULL;
	t_link_element* previous = NULL;

	if ((self->elements_count > index) && (index >= 0)) {
		new_element = list_create_element(data);

		if (index == 0) {
			list_link_element(new_element, self->head);
			self->head = new_element;
		} else {
			next = list_get_element(self, index);
			previous = list_get_element(self, index - 1);

			list_link_element(previous, new_element);
			list_link_element(new_element, next);
		}
		self->elements_count++;
	}
}

/*
 * @NAME: list_replace
 * @DESC: Coloca un valor en una de la posiciones de la lista retornando el valor anterior
 */
void *list_replace(t_list *self, int index, void *data) {
	void *old_data = NULL;

	t_link_element *element = list_get_element(self, index);
	if (element != NULL) {
		old_data = element->data;
		element->data = data;
	}

	return old_data;
}

/*
 * @NAME: list_replace_and_destroy_element
 * @DESC: Coloca un valor en una de la posiciones de la lista liberando el valor anterior
 */
void list_replace_and_destroy_element(t_list *self, int num, void *data, void(*element_destroyer)(void*)) {
	void *old_data = list_replace(self, num, data);
	element_destroyer(old_data);
}

/*
 * @NAME: list_find
 * @DESC: Retorna el primer valor encontrado, el cual haga que condition devuelva != 0
 */
void* list_find(t_list *self, bool(*condition)(void*)) {
	t_link_element *element = list_find_element(self, condition, NULL);
	return element != NULL ? element->data : NULL;
}

/*
 * @NAME: list_iterate
 * @DESC: Itera la lista llamando al closure por cada elemento
 */
void list_iterate(t_list* self, void(*closure)(void*)) {
	t_link_element *element = self->head;
	while (element != NULL) {
		closure(element->data);
		element = element->next;
	}
}

/*
 * @NAME: list_remove
 * @DESC: Remueve un elemento de la lista de una determinada posicion y lo retorna.
 */
void *list_remove(t_list *self, int index) {
	void *data = NULL;
	t_link_element *aux_element = NULL;

	if (self->head == NULL) return NULL;

	aux_element = list_get_element(self, index);
	data = aux_element->data;

	if (index == 0) {
		self->head = aux_element->next;
	} else {
		t_link_element* previous = list_get_element(self, index - 1);
		list_link_element(previous, aux_element->next);
	}

	self->elements_count--;
	free(aux_element);
	return data;
}

/*
 * @NAME: list_remove_by_condition
 * @DESC: Remueve el primer elemento de la lista que haga que condition devuelva != 0.
 */
void* list_remove_by_condition(t_list *self, bool(*condition)(void*)) {
	int index = 0;

	t_link_element* element = list_find_element(self, condition, &index);
	if (element != NULL) {
		return list_remove(self, index);
	}

	return NULL;
}

/*
 * @NAME: list_remove_and_destroy_element
 * @DESC: Remueve un elemento de la lista de una determinada posicion y libera la memoria.
 */
void list_remove_and_destroy_element(t_list *self, int index, void(*element_destroyer)(void*)) {
	void* data = list_remove(self, index);
	element_destroyer(data);
}

/*
 * @NAME: list_remove_and_destroy_by_condition
 * @DESC: Remueve y destruye los elementos de la lista que hagan que condition devuelva != 0.
 */
void list_remove_and_destroy_by_condition(t_list *self, bool(*condition)(void*), void(*element_destroyer)(void*)) {
	void* data = list_remove_by_condition(self, condition);
	element_destroyer(data);
}

/*
 * @NAME: list_size
 * @DESC: Retorna el tamaño de la lista
 */
int list_size(t_list *list) {
	return list->elements_count;
}

/*
 * @NAME: list_is_empty
 * @DESC: Verifica si la lista esta vacia
 */
int list_is_empty(t_list *list) {
	return list_size(list) == 0;
}

/*
 * @NAME: list_clean
 * @DESC: Elimina todos los elementos de la lista.
 */
void list_clean(t_list *self) {
	t_link_element* element;
	while (self->head != NULL) {
		element = self->head;
		self->head = self->head->next;
		free(element);
	}
	self->elements_count = 0;
}

void list_clean_and_destroy_elements(t_list *self, void(*element_destroyer)(void*)){
	list_iterate(self, element_destroyer);
	list_clean(self);
}

/*
 * @NAME: list_destroy
 * @DESC: Destruye una lista
 */
void list_destroy(t_list *self) {
	list_clean(self);
	free(self);
}

/*
 * @NAME: list_destroy_and_destroy_elements
 * @DESC: Destruye una lista y sus elementos
 */
void list_destroy_and_destroy_elements(t_list *self, void(*element_destroyer)(void*)) {
	list_clean_and_destroy_elements(self, element_destroyer);
	free(self);
}


static void list_link_element(t_link_element* previous, t_link_element* next) {
	if (previous != NULL) {
		previous->next = next;
	}
}

static t_link_element* list_create_element(void* data) {
	t_link_element* element = malloc(sizeof(t_link_element));
	element->data = data;
	element->next = NULL;
	return element;
}

static t_link_element* list_get_element(t_list* self, int index) {
	int cont = 0;

	if ((self->elements_count > index) && (index >= 0)) {
		t_link_element *element = self->head;
		while (cont < index) {
			element = element->next;
			cont++;
		}
		return element;
	}
	return NULL;
}

static t_link_element* list_find_element(t_list *self, bool(*condition)(void*), int* index) {
	t_link_element *element = self->head;
	int position = 0;

	while (element != NULL && !condition(element->data)) {
		element = element->next;
		position++;
	}

	if (index != NULL) {
		*index = position;
	}

	return element;
}
