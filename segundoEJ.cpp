#include <iostream>
using namespace std;

int main() {

    int numeroCue;
    int saldoIn;
    int totalCar;
    int totalCred;
    int limiteCred;
    int nuevoSaldo;
    cout << "Ingrese el numero de cuenta del cliente: ";
    cin >> numeroCue;

    cout << "Ingrese el saldo inicial de la cuenta: ";
    cin >> saldoIn;

    cout << "Ingrese el total de cargos del mes: ";
    cin >> totalCar;

    cout << "Ingrese el total de creditos aplicados en el mes: ";
    cin >> totalCred;

    cout << "Ingrese el limite de credito permitido: ";
    cin >> limiteCred;

    nuevoSaldo = saldoIn + totalCar - totalCred;

    cout << "El nuevo saldo de la cuenta " << numeroCue << " es: " << nuevoSaldo << endl;
    if (nuevoSaldo > limiteCred) {
        cout << "Se excedio el limite de su credito." << endl;
    } else {
        cout << "El cliente esta dentro del limite de su credito." << endl;
    }
    return 0;
}