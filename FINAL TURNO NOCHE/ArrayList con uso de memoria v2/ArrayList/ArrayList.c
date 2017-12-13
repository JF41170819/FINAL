#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ArrayList.h"

#define AL_INCREMENT 10
#define AL_INITIAL_VALUE 10

//Bien
int resizeUp(ArrayList* this)
{
    int returnAux = -1;
    void** aux_pElements;

    if(this != NULL)
    {
        aux_pElements = (void**)realloc(this->pElements, sizeof(void*)* (this->reservedSize * AL_INCREMENT));

        if(aux_pElements != NULL)
        {
            this->pElements = aux_pElements;
            this->reservedSize = this->reservedSize + AL_INCREMENT;
            returnAux = 0;
        }
    }

    return returnAux;
}

//Semi-bien
int resizeDown(ArrayList* this)
{
    int returnAux = -1;
    void** aux_pElements;

    if(this != NULL)
    {
        aux_pElements = (void**)realloc(this->pElements, sizeof(void*)* (this->reservedSize - AL_INCREMENT));

        if(aux_pElements != NULL)
        {
            this->pElements = aux_pElements;
            this->reservedSize = this->reservedSize - AL_INCREMENT;
            returnAux = 0;
        }
    }

    return returnAux;
}

//Semi-bien
int expand(ArrayList* this, int index)
{
    int returnAux = -1, i;

    if(this != NULL)
    {
        resizeUp(this);

        for(i = this->size ; i > index ; i--)
        {
            this->pElements[i] = this->pElements[i-1];
        }
        this->size++;

        returnAux = 0;
    }

    return returnAux;
}

//Semi-bien
int contract(ArrayList* this, int index)
{
    int returnAux = -1, i;

    if(this != NULL)
    {
        for(i = index; i < this->size; i++)
        {
            this->pElements[i] = this->pElements[i+1];
        }
        this->size--;
        returnAux = 0;
    }

    return returnAux;
}


/** \brief Allocate a new ArrayList with AL_INITIAL_VALUE elements.
 * \param Void.
 * \return ArrayList* Return (NULL) if Error [if can't allocate memory]
 *                  - (pointer to new ArrayList) if ok
 */
ArrayList* al_newArrayList(void)
{
    ArrayList* returnAux = NULL;
    ArrayList* this;

    void** aux_pElements;
    this = (ArrayList *)malloc(sizeof(ArrayList));

    if(this != NULL)
    {
        aux_pElements = (void**)malloc(sizeof(void*)*AL_INITIAL_VALUE);

        if(aux_pElements != NULL)
        {
            this->size = 0;
            this->pElements = aux_pElements;
            this->reservedSize = AL_INITIAL_VALUE;
            this->add = al_add;
            this->len = al_len;
            this->set = al_set;
            this->remove = al_remove;
            this->clear = al_clear;
            this->clone = al_clone;
            this->get = al_get;
            this->contains = al_contains;
            this->push = al_push;
            this->indexOf = al_indexOf;
            this->isEmpty = al_isEmpty;
            this->pop = al_pop;
            this->subList = al_subList;
            this->containsAll = al_containsAll;
            this->deleteArrayList = al_deleteArrayList;
            this->sort = al_sort;
            returnAux = this;
        }
        else
            free(this);
    }

    return returnAux;

}


/** \brief  Add an element to ArrayList and if is
 *          nessesary resize the array
 * \param this ArrayList* Pointer to ArrayList
 * \param pElement void* Pointer to element
 * \return int Return (-1) if Error [this or pElement are NULL pointer] - (0) if Ok
 *
 */
int al_add(ArrayList* this, void* pElement)
{
    int returnAux = -1;
    void** aux_pElements;
    int resizeReturn=0;

    if(this != NULL && pElement != NULL)
    {
        if(this->size==this->reservedSize)
        {
            resizeReturn = resizeUp(this);
        }
        if(resizeReturn==0)
        {
            this->pElements[this->size] = pElement;
            this->size++;
            returnAux = 0;
        }
    }

    return returnAux;
}

/** \brief  Delete ArrayList
 * \param this ArrayList* Pointer to ArrayList
 * \return int Return length of array or (-1) if Error [this is NULL pointer]
 *
 */
int al_deleteArrayList(ArrayList* this)
{
    int returnAux = -1;

    if(this != NULL)
    {
        if(this->pElements != NULL)
        {
            free(this->pElements);
            returnAux = 0;
        }
        free(this);
    }

    return returnAux;
}

/** \brief  Measure ArrayList
 * \param this ArrayList* Pointer to ArrayList
 * \return int Return length of array or (-1) if Error [this is NULL pointer]
 *
 */
int al_len(ArrayList* this)
{
    int returnAux = -1;

    if(this != NULL)
    {
        returnAux = this->size;
    }

    return returnAux;
}


/** \brief  Get an element by index
 * \param this ArrayList* Pointer to ArrayList
 * \param index int Index of the element
 * \return void* Return (NULL) if Error [this is NULL pointer or invalid index] - (Pointer to element) if Ok
 *
 */
void* al_get(ArrayList* this, int index)
{
    void* returnAux = NULL;

    if(this != NULL)
    {
        if(index < this->size && index >= 0)
        {
            returnAux = this->pElements[index];
        }
    }

    return returnAux;
}


/** \brief  Find if this contains at least one element pElement
 * \param this ArrayList* Pointer to ArrayList
 * \param pElement void* Pointer to element
 * \return int Return (-1) if Error [this or pElement are NULL pointer]
 *                  - ( 0) if Ok but not found a element
 *                  - ( 1) if this list contains at least one element pElement
 *
 */
int al_contains(ArrayList* this, void* pElement)
{
    int returnAux = -1, i;

    if(this != NULL && pElement != NULL)
    {
        returnAux = 0;

        for(i = 0; i<this->size ; i++)
        {
            //this->pElements[i]==pElement
            if(*(this->pElements+i)==pElement)
            {
                returnAux = 1;
                break;
            }
        }
    }

    return returnAux;
}


/** \brief  Set a element in this at index position
 * \param this ArrayList* Pointer to ArrayList
 * \param index int Index of the element
 * \param pElement void* Pointer to element
 * \return int Return (-1) if Error [this or pElement are NULL pointer or invalid index]
 *                  - ( 0) if Ok
 *
 */
int al_set(ArrayList* this, int index,void* pElement)
{
    int returnAux = -1;
    if(this != NULL && pElement != NULL && index>=0 && index<=this->size)
    {
        if(index==this->size)
        {
            if(this->add(this, pElement)==0)
                returnAux = 0;

        }
        else
        {
            if(index<this->size)
            {
                this->pElements[index]=pElement;
                returnAux = 0;
            }
        }
    }
    return returnAux;
}


/** \brief  Remove an element by index
 * \param this ArrayList* Pointer to ArrayList
 * \param index int Index of the element
 * \return int Return (-1) if Error [this is NULL pointer or invalid index]
 *                  - ( 0) if Ok
 */
int al_remove(ArrayList* this,int index)
{
    int returnAux = -1;

    if(this != NULL && this->pElements != NULL)
    {
        if(index<this->size && index>=0)
        {
            if(contract(this, index)==0)
                returnAux = 0;
        }
    }

    return returnAux;
}



/** \brief Removes all of the elements from this list
 * \param this ArrayList* Pointer to ArrayList
 * \return int Return (-1) if Error [this is NULL pointer]
 *                  - ( 0) if Ok
 */
int al_clear(ArrayList* this)
{
    int returnAux = -1;

    if(this != NULL)
    {
        if((resizeDown(this))==0)
            returnAux = 0;
    }
    return returnAux;
}



/** \brief Returns an array containing all of the elements in this list in proper sequence
 * \param this ArrayList* Pointer to ArrayList
 * \return ArrayList* Return  (NULL) if Error [this is NULL pointer]
 *                          - (New array) if Ok
 */
ArrayList* al_clone(ArrayList* this)
{
    ArrayList* returnAux = NULL;

    if(this != NULL)
    {
        returnAux = al_newArrayList();

        if(returnAux != NULL)
        {
            returnAux = this;
            returnAux->size = this->size;
            returnAux->pElements = this->pElements;
        }
    }

    return returnAux;
}




/** \brief Inserts the element at the specified position
 * \param this ArrayList* Pointer to ArrayList
 * \param index int Index of the element
 * \param pElement void* Pointer to element
 * \return int Return (-1) if Error [this or pElement are NULL pointer or invalid index]
 *                  - ( 0) if Ok
 */
int al_push(ArrayList* this, int index, void* pElement)
{
    int returnAux = -1;

    if(this != NULL && index>=0 && pElement != NULL)
    {
        if(index == this->size)
        {
            if(this->add(this, pElement)==0)
                returnAux = 0;
        }
        else if(index<this->size && expand(this, index)==0 && this->set(this, index, pElement)==0)
        returnAux = 0;
    }

    return returnAux;
}




/** \brief Returns the index of the first occurrence of the specified element
 * \param this ArrayList* Pointer to ArrayList
 * \param pElement void* Pointer to element
 * \return int Return (-1) if Error [this or pElement are NULL pointer] - (index to element) if Ok
 */
int al_indexOf(ArrayList* this, void* pElement)
{
    int returnAux = -1, i;

    if(this != NULL && this->pElements != NULL)
    {
        for(i = 0; i<this->size ; i++)
        {
            if(pElement == this->pElements[i])
            {
                returnAux = i;
            }
        }
    }

    return returnAux;
}



/** \brief Returns true if this list contains no elements.
 * \param this ArrayList* Pointer to ArrayList
 * \return int Return (-1) if Error [this is NULL pointer] - (0) if Not Empty - (1) if is Empty
 */
int al_isEmpty(ArrayList* this)
{
    int returnAux = -1;

    if(this != NULL)
    {
        if(this->len(this) == 0)
        {
            returnAux = 1;
        }
        else
        returnAux = 0;
    }

    return returnAux;
}




/** \brief Remove the item at the given position in the list, and return it.
 * \param this ArrayList* Pointer to ArrayList
 * \param index int Index of the element
 * \return int Return (NULL) if Error [this is NULL pointer or invalid index]
 *                  - ( element pointer) if Ok
 */
void* al_pop(ArrayList* this,int index)
{
    void* returnAux = NULL;

    if(this != NULL && this->pElements != NULL)
    {
        if(index<this->size && index>= 0)
        {
            returnAux = *(this->pElements + index); //Devuelve el elemento dentro del puntero.
            contract(this, index);
        }
    }

    return returnAux;
}


/** \brief Returns a new ArrayList with a portion of this between the specified
 *         fromIndex, inclusive, and toIndex, exclusive.
 * \param this ArrayList* Pointer to ArrayList
 * \param from int Initial index of the element (inclusive)
 * \param to int Final index of the element (exclusive)
 * \return int Return (NULL) if Error [this is NULL pointer or invalid 'from' or invalid 'to']
 *                  - ( pointer to new array) if Ok
 */
ArrayList* al_subList(ArrayList* this,int from,int to)
{
    ArrayList* returnAux = NULL;
    void* aux_pElement = NULL;
    int i;

    if(this != NULL && from>=0 && from<=to && to<=this->size)
    {
        returnAux = al_newArrayList();

        if(returnAux != NULL)
        {
            for(i = from; i<to; i++)
            {
                if(resizeUp(returnAux) == 0)
                {
                    aux_pElement = this->get(this, i);
                    returnAux->add(returnAux, aux_pElement);
                }
            }
        }
    }

    return returnAux ;
}


/** \brief Returns true if this list contains all of the elements of this2
 * \param this ArrayList* Pointer to ArrayList
 * \param this2 ArrayList* Pointer to ArrayList
 * \return int Return (-1) if Error [this or this2 are NULL pointer]
 *                  - (0) if Not contains All - (1) if is contains All
 */
int al_containsAll(ArrayList* this,ArrayList* this2)
{
    int returnAux = -1;
    int i, j, cont = 0;

    if(this != NULL && this2 != NULL)
    {
        for(i = 0; i>this->size; i++)
        {
            for(j = 0; j>this2->size ; j++)
            {
                if(this->pElements[i] == this2->pElements[j])
                {
                    cont++;
                    break;
                }
            }
        }
        if(cont==(this2->size))
        returnAux = 1;
        else
        returnAux = 0;
    }

    return returnAux;
}


/** \brief Sorts objects of list, use compare pFunc
 * \param this ArrayList* Pointer to ArrayList
 * \param pFunc (*pFunc) Pointer to fuction to compare elements of ArrayList
 * \param order int  [1] indicate UP - [0] indicate DOWN
 * \return int Return (-1) if Error [this or pFunc are NULL pointer]
 *                  - (0) if ok
 */
int al_sort(ArrayList* this, int (*pFunc)(void*,void*), int order)
{
    int returnAux = -1;
    void* pElementA;
    void* pElementB;
    void* aux_pElement = NULL;

    int i, j;

    if(this != NULL && pFunc != NULL && (order == 1 || order == 0))
    {
        for(i = 0; i<this->size-1 ; i++)
        {
            for(j = i + 1; j<this->size ; j++)
            {
                pElementA = this->get(this, i);
                pElementB = this->get(this, j);
                if(order == 1)
                {
                    if(pFunc(pElementA, pElementB)== 1)
                    {
                        aux_pElement = this->pElements[i];
                        this->pElements[i] = this->pElements[j];
                        this->pElements[j] = aux_pElement;
                    }
                }
                else
                {
                    if(pFunc(pElementA, pElementB)== -1)
                    {
                        aux_pElement = this->pElements[i];
                        this->pElements[i] = this->pElements[j];
                        this->pElements[j] = aux_pElement;
                    }
                }
            }
        }

        returnAux = 0;
    }
    return returnAux;
}
