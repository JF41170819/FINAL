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


    /*for(i = 0; i<listaClientes->len(listaClientes) ; i++)
    {
        eCliente* unCliente = (eCliente*)listaClientes->get(listaClientes, i);

        if(unCliente->id<0)
        {
            listaClientes->deleteArrayList(listaClientes);
            ArrayList* listaClientes = al_newArrayList();
        }
    }*/

    int returnAux = -1;


    int i;
    char auxID[200];
    char auxNombre[200];
    char auxApellido[200];
    char auxDNI[200];

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
int alta_cliente(ArrayList* listaClientes)
{

    int index_lastID = listaClientes->len(listaClientes)-1;
    int id;

    eCliente* ultimoCliente = listaClientes->get(listaClientes, index_lastID);

    if(ultimoCliente != NULL)
    {
        id = ultimoCliente->id;
        id++; //Le sumo uno
    }


    char auxNombre[200];
    char auxApellido[200];
    long long int auxDni;

    printf("Ingrese su nombre: ");
    scanf("%s",auxNombre);

    printf("Ingrese su apellido: ");
    scanf("%s",auxApellido);

    printf("Ingrese DNI:");
    scanf("%lli", &auxDni);

    //Se puede hacer verificacion para DNI

    while(auxDni < 0 || auxDni > 99999999)
    {
        printf("Error!, DNI invalido!!!\n");
        printf("Ingrese DNI:");
        scanf("%lli", &auxDni);
    }

    int i;
    for(i = 0; i<listaClientes->len(listaClientes); i++)
    {
        eCliente* otroCliente = (eCliente*)listaClientes->get(listaClientes, i);

        while(auxDni == otroCliente->dni)
        {
            printf("Error!, ese DNI ya esta ingresado!!!\n");
            printf("Ingrese DNI:");
            scanf("%lli", &auxDni);
        }
    }


    eCliente* unCliente;

    unCliente = new_cliente();


    /** USAR SETTERS **/
    if(unCliente != NULL)
    {
        unCliente->id = id;
        strcpy(unCliente->nombre, auxNombre);
        strcpy(unCliente->apellido, auxApellido);
        unCliente->dni = auxDni;

        listaClientes->add(listaClientes, unCliente);
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
    long long int auxDni;

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

                printf("Ingrese DNI:");
                scanf("%lli", &auxDni);

                //Se puede hacer verificacion para DNI

                while(auxDni < 0 || auxDni > 99999999)
                {
                    printf("Error!, DNI invalido!!!\n");
                    printf("Ingrese DNI:");
                    scanf("%lli", &auxDni);
                }

                int x;
                for(x = 0; x<listaClientes->len(listaClientes); i++)
                {
                    eCliente* otroCliente = (eCliente*)listaClientes->get(listaClientes, x);

                    while(auxDni == otroCliente->dni)
                    {
                        printf("Error!, ese DNI ya esta ingresado!!!\n");
                        printf("Ingrese DNI:");
                        scanf("%lli", &auxDni);
                    }
                }

                aux->id = id;
                strcpy(aux->nombre, auxNombre);
                strcpy(aux->apellido, auxApellido);
                aux->dni = auxDni;

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
int baja_cliente(ArrayList* listaClientes, ArrayList* listaVentas)
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
