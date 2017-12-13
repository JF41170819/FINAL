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
        returnAux->dni = 0;
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
        returnAux->codProducto = 0;
        returnAux->cantidad = 0;
        returnAux->precioUnitario = 0;
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

        printf("NOMBRE: %s\tAPELLIDO: %s\tID:%d\tDNI:%lli\n",unCliente->nombre, unCliente->apellido,unCliente->id,unCliente->dni);
    }

}
/*** -------- ***/
/*** IMPRIMIR ***/
/*** -------- ***/
void imprimir_ventas(ArrayList* listaVentas, ArrayList* listaClientes)
{

    eVenta* unaVenta;
    eCliente* unCliente;

    int flag;

    int i,j;

    for(i = 0; i<listaClientes->len(listaClientes) ; i++)
    {
        unCliente = (eCliente*)listaClientes->get(listaClientes, i);

        for(j = 0; j<listaVentas->len(listaVentas); j++)
        {
            unaVenta = (eVenta*)listaVentas->get(listaVentas, j);

            flag = listaVentas->contains(listaVentas,listaClientes->get(listaClientes, i));

            if(flag)
            {
                printf("ID DE VENTA: %d\t NOMBRE: %s\t APELLIDO: %s\t CODIGO: %d\tMONTO:asd\n",unaVenta->idVenta, unCliente->nombre, unCliente->apellido, unaVenta->codProducto);
            }

        }


    }

}
/*** -------------- ***/
/*** ABRIR ARCHIVO  ***/
/*** -------------- ***/
FILE* abrir_Archivo(char* path, int modo)
{
    FILE* pFile;

    if(modo == 0) //Leer y escribir
    {

        if((pFile = fopen(path,"r+")) == NULL)
        {
            if((pFile = fopen(path,"w+")) == NULL)
            {
                printf("Error, no se ha podido cargar el archivo!\n");
            }

        }

    }
    else if(modo == 1)//Leer
    {
        if((pFile = fopen(path,"r+")) == NULL)
        {
            printf("Error, no se ha podido cargar el archivo!\n");
        }
    }
    else if(modo == 2)//Escribir
    {
        if((pFile = fopen(path,"w+")) == NULL)
        {
            printf("Error, no se ha podido cargar el archivo!\n");
        }
    }
    return pFile;
}
/*** -------------- ***/
/*** PARSER CLIENTE ***/
/*** ------------- ***/
int parser_cliente(ArrayList* listaClientes)
{
    int returnAux = -1;

    FILE* pFile = abrir_Archivo("clientes.txt",0);

    char auxID[200];
    char auxNombre[200];
    char auxApellido[200];
    char auxDNI[200];

    if(pFile != NULL && listaClientes != NULL)
    {
        while(!feof(pFile))
        {
            fscanf(pFile, "%[^,],%[^,],%[^,],%[^\n]\n", auxID, auxNombre, auxApellido, auxDNI);

            eCliente* unCliente = new_cliente();

            if(unCliente != NULL)
            {
                unCliente->id = atoi(auxID);
                strcpy(unCliente->nombre, auxNombre);
                strcpy(unCliente->apellido, auxApellido);
                unCliente->dni = atoll(auxDNI);
            }

            if(unCliente->dni != 0)
            {
                listaClientes->add(listaClientes,unCliente);
            }
        }

        fclose(pFile);

        returnAux = 0;
    }

    return returnAux;
}
/*** -------------- ***/
/*** PARSER VENTA ***/
/*** ------------- ***/
int parser_venta(ArrayList* listaVentas, ArrayList* listaClientes)
{

    int returnAux = -1;

    char auxIdVenta[200];
    char auxIdCliente[200];
    char auxCodProducto[200];
    char auxCantidad[200];
    char auxPrecioUnitario[200];
    int i;
    int flag = 1;
    int index;

    eCliente* aux;


    FILE* pFile = abrir_Archivo("ventas.txt",0);

    if(pFile != NULL && listaVentas != NULL)
    {
        while(!feof(pFile))
        {
            fscanf(pFile, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", auxIdVenta, auxIdCliente, auxCodProducto, auxCantidad, auxPrecioUnitario);

            //printf("%s,%s,%s,%s,%s\n", auxIdVenta, auxIdCliente, auxCodProducto, auxCantidad, auxPrecioUnitario);

            for(i=0; i<listaClientes->len(listaClientes); i++)
            {
                aux = (eCliente*)listaClientes->get(listaClientes,i);

                if(aux->id == atoi(auxIdCliente))
                {
                    flag=0;
                    index=i;
                    break;
                }
            }
            if(!flag)
            {
                eVenta* unaVenta = new_venta();

                unaVenta->idVenta = atoi(auxIdVenta);
                unaVenta->idCliente = aux->id;
                unaVenta->codProducto = atoi(auxCodProducto);
                unaVenta->cantidad = atoi(auxCantidad);
                unaVenta->precioUnitario = atof(auxPrecioUnitario);

                listaVentas->add(listaVentas,unaVenta);
            }
            else
            {

            eVenta* unaVenta = new_venta();

            if(unaVenta != NULL)
            {
                unaVenta->idVenta = atoi(auxIdVenta);
                unaVenta->idCliente = atoi(auxIdCliente);
                unaVenta->codProducto = atoi(auxCodProducto);
                unaVenta->cantidad = atoi(auxCantidad);
                unaVenta->precioUnitario = atof(auxPrecioUnitario);

                listaVentas->add(listaVentas,unaVenta);
            }

            }

        }

        fclose(pFile);

        returnAux = 0;
    }

    return returnAux;
}
int obtener_ultimoID_cliente(ArrayList* listaClientes)
{
    /*** OBTENGO ULTIMO ID ***/

    int index_lastID = listaClientes->len(listaClientes)-1; //Copio el indice del ultimo id

    eCliente* ultimoCliente = (eCliente*)listaClientes->get(listaClientes, index_lastID); // Obtengo el id

    int id;

    if(ultimoCliente != NULL)
    {
        id = ultimoCliente->id;
        id++; //Le sumo uno
    }

    return id;

}
int obtener_ultimoID_venta(ArrayList* listaVentas)
{
    /*** OBTENGO ULTIMO ID ***/

    int index_lastID = listaVentas->len(listaVentas)-1; //Copio el indice del ultimo id

    eVenta* ultimaVenta = (eVenta*)listaVentas->get(listaVentas, index_lastID); // Obtengo el id

    int id;

    if(ultimaVenta != NULL)
    {
        id = ultimaVenta->idVenta;
        id++; //Le sumo uno
    }

    return id;

}
/*** ------------ ***/
/*** ALTA CLIENTE ***/
/*** ------------ ***/
int alta_cliente(ArrayList* listaClientes)
{
    int returnAux = -1;

    FILE* pFile = abrir_Archivo("clientes.txt",0);

    int id = obtener_ultimoID_cliente(listaClientes);

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

    if(pFile != NULL)
    {
        fseek(pFile, 0L, SEEK_END);
        fprintf(pFile,"%d,%s,%s,%lli\n", unCliente->id, unCliente->nombre, unCliente->apellido,unCliente->dni);
        fclose(pFile);

        returnAux = 0;
    }

    return returnAux;

}
int modif_cliente(ArrayList* listaClientes)
{
    int returnAux = -1;

    int id,i,x;
    char auxNombre[200];
    char auxApellido[200];
    long long int auxDni;

    FILE* pFile = abrir_Archivo("clientes.txt",1);

    FILE* pFile2 = abrir_Archivo("clientes2.txt",2);

    eCliente* aux;

    if(pFile != NULL && pFile2 != NULL && listaClientes != NULL)
    {
        printf("Ingrese ID a modificar: ");
        scanf("%d", &id);

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

                for(x = 0; x<listaClientes->len(listaClientes); x++)
                {
                    eCliente* otroCliente = (eCliente*)listaClientes->get(listaClientes, x);

                    while(auxDni == otroCliente->dni)
                    {
                        printf("Error!, ese DNI ya esta ingresado!!!\n");
                        printf("Ingrese DNI:");
                        scanf("%lli", &auxDni);
                    }
                }

                if(aux != NULL)
                {
                    aux->id = id;
                    strcpy(aux->nombre, auxNombre);
                    strcpy(aux->apellido, auxApellido);
                    aux->dni = auxDni;
                }

            }

            fseek(pFile2,0L, SEEK_END);
            fprintf(pFile2,"%d,%s,%s,%lli\n", aux->id, aux->nombre, aux->apellido, aux->dni);

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

    FILE* pFile = abrir_Archivo("clientes.txt",1);

    FILE* pFile2 = abrir_Archivo("clientes2.txt",2);

    int id,i,j;
    char auxNombre[200];
    char auxApellido[200];

    int flag = 0;

    if(pFile != NULL && pFile2 != NULL)
    {
        printf("Ingrese ID del cliente a borrar: ");
        scanf("%d",&id);

        for(i = 0; i<listaClientes->len(listaClientes); i++)
        {
            eCliente* cliente = (eCliente*)listaClientes->get(listaClientes, i);

            if(cliente->id == id)
            {
                //El id coincide

                for(j = 0; j<listaVentas->len(listaVentas); j++)
                {
                    eVenta* venta = (eVenta*)listaVentas->get(listaVentas, j);

                    if(cliente->id == venta->idCliente)//Si el id tiene una venta en curso
                    {
                        flag = 1;
                    }
                }

                if(flag == 0)
                {
                    listaClientes->remove(listaClientes,i);
                }
            }
        }

        int x;
        for(x = 0; x<listaClientes->len(listaClientes); x++)
        {
            eCliente* unCliente = (eCliente*)listaClientes->get(listaClientes, x);
            fseek(pFile2,0L, SEEK_END);
            fprintf(pFile2,"%d,%s,%s,%lli\n", unCliente->id, unCliente->nombre, unCliente->apellido, unCliente->dni);
        }

        fclose(pFile);
        fclose(pFile2);
        remove("clientes.txt");
        rename( "clientes2.txt", "clientes.txt" );

        returnAux = 0;
    }

    return returnAux;
}
/*** ------------ ***/
/*** ALTA CLIENTE ***/
/*** ------------ ***/
int alta_venta(ArrayList* listaVentas, ArrayList* listaClientes)
{
    int returnAux = -1;

    FILE* pFile = abrir_Archivo("ventas.txt",0);

    int idVenta = obtener_ultimoID_venta(listaVentas);

    int auxId,i; //id cliente
    int auxCodProducto;
    int auxCantidad;

    eCliente* otroCliente;

    if(pFile != NULL && listaVentas != NULL && listaClientes != NULL)
    {
        printf("Ingrese su id: ");
        scanf("%d",&auxId);

        for(i = 0; i<listaClientes->len(listaClientes); i++)
        {
            otroCliente = (eCliente*)listaClientes->get(listaClientes, i);

            if(auxId == otroCliente->id)
            {
                printf("ID encontrado!.\n");

                printf("Ingrese codigo del producto a comprar: ");
                scanf("%d",&auxCodProducto);

                printf("Ingrese cantidad: ");
                scanf("%d",&auxCantidad);

                eVenta* nuevaVenta = new_venta();

                if(nuevaVenta != NULL)
                {
                    nuevaVenta->idCliente = auxId;
                    nuevaVenta->idVenta = idVenta;
                    nuevaVenta->codProducto = auxCodProducto;
                    nuevaVenta->cantidad = auxCantidad;
                    nuevaVenta->precioUnitario = 9;

                    listaVentas->add(listaVentas, nuevaVenta);
                }

                fseek(pFile,0L, SEEK_END);
                fprintf(pFile,"%d,%d,%d,%d,%f\n", nuevaVenta->idVenta, nuevaVenta->idCliente, nuevaVenta->codProducto, nuevaVenta->cantidad,nuevaVenta->precioUnitario);
            }
        }

        fclose(pFile);

        returnAux = 0;
    }

    return returnAux;
}
int baja_venta(ArrayList* listaVentas)
{
    int returnAux = -1;

    FILE* pFile = abrir_Archivo("ventas.txt",1);

    FILE* pFile2 = abrir_Archivo("ventas2.txt",2);

    int id;
    printf("Ingrese el ID de la venta a eliminar: ");
    scanf("%d", &id);

    int i;

    if(pFile != NULL && pFile2 != NULL)
    {
        for(i=0 ; i<listaVentas->len(listaVentas); i++)
        {
            eVenta* venta = (eVenta*)listaVentas->get(listaVentas, i);

            if(venta->idVenta == id)
            {
                printf("Venta eliminada");
                listaVentas->remove(listaVentas, i);
            }
            else
            {
                fseek(pFile2,0L, SEEK_END);
                fprintf(pFile2,"%d,%d,%d,%d,%f\n", venta->idVenta, venta->idCliente, venta->codProducto, venta->cantidad,venta->precioUnitario);
            }


        }

        fclose(pFile);
        fclose(pFile2);
        remove("ventas.txt");
        rename( "ventas2.txt", "ventas.txt" );

        returnAux = 0;
    }

    return returnAux;
}
