typedef struct
{
    int id;
    char nombre[200];
    char apellido[200];
    long long int dni;
}eCliente;

typedef struct
{
    int idVenta;
    int idCliente;

}eVenta;
