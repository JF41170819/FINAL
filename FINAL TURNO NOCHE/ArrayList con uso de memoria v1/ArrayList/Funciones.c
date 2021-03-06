#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ArrayList.h"
#include "Funciones.h"

/*** ------------- ***/
/*** CONSTRUCTOR 1 ***/
/*** ------------- ***/
eCliente* new_cliente()
{
    eCliente* returnAux;

    returnAux = (eCliente*)malloc(sizeof(eCliente));

    if(returnAux != NULL)
    {
        returnAux->id = 0;
        strcpy(returnAux->nombre, "");
        strcpy(returnAux->apellido, "");
    }

    return returnAux;
}

/*** ------------- ***/
/*** CONSTRUCTOR 2 ***/
/*** ------------- ***/
eVenta* new_venta()
{
    eVenta* returnAux;

    returnAux = (eVenta*)malloc(sizeof(eVenta));

    if(returnAux != NULL)
    {
        returnAux->idCliente = 0;
        returnAux->idVenta = 0;
    }

    return returnAux;
}
/*** -------- ***/
/*** IMPRIMIR ***/
/*** -------- ***/
void imprimir_clientes(ArrayList* lista)
{

    eCliente* unCliente;

    int i;

    for(i = 0; i<lista->len(lista) ; i++)
    {
        unCliente = (eCliente*)lista->get(lista, i);

        printf("NOMBRE: %s\tAPELLIDO: %s\tID:%d\n",unCliente->nombre, unCliente->apellido,unCliente->id);
    }

}
/*** -------------- ***/
/*** PARSER CLIENTE ***/
/*** ------------- ***/
int parser_cliente(ArrayList* listaClientes)
{
    int i;

    for(i = 0; i<listaClientes->len(listaClientes) ; i++)
    {
        eCliente* unCliente = (eCliente*)listaClientes->get(listaClientes, i);

        if(unCliente->id>0)
        {
            listaClientes->deleteArrayList(listaClientes);
            ArrayList* listaClientes = al_newArrayList();
        }
    }


    int returnAux = -1;

    char auxID[200];
    char auxNombre[200];
    char auxApellido[200];

    FILE* pFile;

    if((pFile = fopen("clientes.txt","r+")) == NULL)
    {
        if((pFile = fopen("clientes.txt","w+")) == NULL)
        {
            printf("Error, no se ha podido cargar el archivo!\n");
        }

    }


    if(pFile != NULL && listaClientes != NULL)
    {
        while(!feof(pFile))
        {
            fscanf(pFile, "%[^,],%[^,],%[^\n]\n", auxID, auxNombre, auxApellido);

            //printf("NOMBRE: %s -- EMAIL: %s\n",auxNombre, auxEmail);

            eCliente* unCliente = new_cliente();

            if(unCliente != NULL)
            {
                unCliente->id = atoi(auxID);
                strcpy(unCliente->nombre, auxNombre);
                strcpy(unCliente->apellido, auxApellido);
            }

            if(!(strcmp(unCliente->nombre,"")==0 || strcmp(unCliente->apellido,"")==0))
            {
                listaClientes->add(listaClientes,unCliente);
            }


        }
        fclose(pFile);
        returnAux = 0;
    }

    return returnAux;
}
/*** ------------ ***/
/*** ALTA CLIENTE ***/
/*** ------------ ***/
int alta_cliente(ArrayList* listaClientes, int contadorId)
{

    int id = contadorId; //Guardo el id que recibo con el contador del Main.

    char auxNombre[200];
    char auxApellido[200];

    printf("Ingrese su nombre: ");
    scanf("%s",auxNombre);

    printf("Ingrese su apellido: ");
    scanf("%s",auxApellido);

    //Se puede hacer verificacion para DNI

    eCliente* unCliente;

    unCliente = new_cliente();

    /** USAR SETTERS **/
    if(unCliente != NULL)
    {
        unCliente->id = id;
        strcpy(unCliente->nombre, auxNombre);
        strcpy(unCliente->apellido, auxApellido);
    }

    /** USAR FUNCION ARCHIVO **/

    FILE* pFile;

    if((pFile = fopen("clientes.txt","r+")) == NULL)
    {
        if((pFile = fopen("clientes.txt","w+")) == NULL)
        {
            printf("Error, no se ha podido abrir el archivo!\n");
        }
    }


    //Guardado de archivo

    if(pFile != NULL)
    {
        fseek(pFile, 0L, SEEK_END);
        fprintf(pFile,"%d,%s,%s\n", unCliente->id, unCliente->nombre, unCliente->apellido);
        fclose(pFile);

        printf("Persona cargada correctamente\n");
    }
    return 0;
}
int modif_cliente(ArrayList* listaClientes)
{

    /** USAR FUNCION ARCHIVO **/

    FILE* pFile;

    if((pFile = fopen("clientes.txt","r+")) == NULL)
    {
        printf("Error, no se ha podido abrir el archivo!\n");
    }

    FILE* pFile2;

    if((pFile2 = fopen("clientes2.txt","w+")) == NULL)
    {
        printf("Error, no se ha podido crear el archivo!\n");
    }

    int returnAux = -1;

    char auxID [200];
    char auxNombre[200];
    char auxApellido[200];

    imprimir_clientes(listaClientes);

    printf("Ingrese ID del cliente a modificar: ");
    int id;
    scanf("%d",&id);

    eCliente* aux;

    int i;
    if(pFile != NULL && pFile2 != NULL && listaClientes != NULL)
    {

        for(i = 0; i<listaClientes->len(listaClientes); i++)
        {
            aux = (eCliente*)listaClientes->get(listaClientes, i);

            if((aux->id == id))
            {
                printf("Ingrese nuevo nombre: ");
                scanf("%s",auxNombre);

                printf("Ingrese nuevo apellido: ");
                scanf("%s",auxApellido);

                aux->id = id;
                strcpy(aux->nombre, auxNombre);
                strcpy(aux->apellido, auxApellido);

            }

            fseek(pFile2,0L, SEEK_END);
            fprintf(pFile2,"%d,%s,%s\n", aux->id, aux->nombre, aux->apellido);

        }

        fclose(pFile);
        fclose(pFile2);
        remove("clientes.txt");
        rename( "clientes2.txt", "clientes.txt" );

        returnAux = 0;
    }

    return returnAux;
}
int baja_cliente(ArrayList* listaClientes)
{

    int returnAux = -1;

    /** USAR FUNCION ARCHIVO **/

    FILE* pFile;

    if((pFile = fopen("clientes.txt","r+")) == NULL)
    {
        printf("Error, no se ha podido abrir el archivo!\n");
    }

    FILE* pFile2;

    if((pFile2 = fopen("clientes2.txt","w+")) == NULL)
    {
        printf("Error, no se ha podido crear el archivo!\n");
    }

    char auxID [200];
    char auxNombre[200];
    char auxApellido[200];


    printf("Ingrese ID del cliente a borrar: ");
    int id;
    scanf("%d",&id);

    int i;

    for(i = 0; i<listaClientes->len(listaClientes); i++)
    {
        eCliente* aux = (eCliente*)listaClientes->get(listaClientes, i);

        if(!(aux->id == id))
        {
            /** USAR FUNCION ARCHIVO **/
            fseek(pFile2,0L, SEEK_END);
            fprintf(pFile2,"%d,%s,%s\n", aux->id, aux->nombre, aux->apellido);

        }
        else
        {
            listaClientes->remove(listaClientes,i);
        }

    }

    fclose(pFile);
    fclose(pFile2);
    remove("clientes.txt");
    rename( "clientes2.txt", "clientes.txt" );

    return returnAux;
}
