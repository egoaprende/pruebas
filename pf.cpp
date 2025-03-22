#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

//las configuraciones de propeties y tasks son necesarias para que corra el programa

using namespace std;

// Estructura para almacenar informacion de los productos
struct Producto {
    int codigo;
    string nombre;
    double precio;
    int cantidad;
    Producto* siguiente;
};

// Clase para gestionar el inventario
class Inventario {
private:
    Producto* cabeza; // Puntero al primer producto
public:
    Inventario() : cabeza(nullptr) {}

    // Metodo para agregar un producto al inventario
    void agregarProducto(int codigo, string nombre, double precio, int cantidad) {
        Producto* nuevo = new Producto{codigo, nombre, precio, cantidad, cabeza};
        cabeza = nuevo;
    }

    // Metodo para eliminar un producto del inventario
    void eliminarProducto(int codigo) {
        Producto* actual = cabeza;
        Producto* anterior = nullptr;

        while (actual != nullptr && actual->codigo != codigo) {
            anterior = actual;
            actual = actual->siguiente;
        }

        if (actual == nullptr) return; // No se encontro el producto

        if (anterior == nullptr) {
            cabeza = actual->siguiente;
        } else {
            anterior->siguiente = actual->siguiente;
        }

        delete actual;
    }

    // Obtener la lista de productos en formato de vector de strings
    vector<string> obtenerInventario() {
        vector<string> lista;
        Producto* actual = cabeza;

        while (actual != nullptr) {
            lista.push_back(to_string(actual->codigo) + "\t" + actual->nombre + "\t" + to_string(actual->precio) + "\t" + to_string(actual->cantidad));
            actual = actual->siguiente;
        }
        return lista;
    }

    // buscar un producto por su codigo
    string buscarProducto(int codigo) {
        Producto* actual = cabeza;
        while (actual != nullptr) {
            if (actual->codigo == codigo) {
                return to_string(actual->codigo) + "\t" + actual->nombre + "\t" + to_string(actual->precio) + "\t" + to_string(actual->cantidad);
            }
            actual = actual->siguiente;
        }
        return "Producto no encontrado.";
    }

    // Destructor para liberar memoria
    ~Inventario() {
        Producto* actual = cabeza;
        while (actual != nullptr) {
            Producto* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
};

Inventario tienda; // Instancia del inventario
HWND hEditCodigo, hEditNombre, hEditPrecio, hEditCantidad; // controles de la interfaz

// Funcion para mostrar mensajes en una ventana emergente
void mostrarMensaje(HWND hWnd, const char* mensaje) {
    MessageBox(hWnd, mensaje, "Información", MB_OK | MB_ICONINFORMATION);
}

// Funcion para mostrar el inventario en un cuadro de mensaje
void mostrarInventario(HWND hWnd) {
    vector<string> lista = tienda.obtenerInventario();
    string inventario = "Codigo\tNombre\tPrecio\tCantidad\n";
    for (const string& item : lista) {
        inventario += item + "\n";
    }
    MessageBox(hWnd, inventario.c_str(), "Inventario", MB_OK | MB_ICONINFORMATION);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Función principal del programa con windous api
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "InventarioApp";
    RegisterClass(&wc);

    // Creacion de la ventana principal
    HWND hWnd = CreateWindow("InventarioApp", "Inventario de tienda", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, NULL, NULL, hInstance, NULL);
    if (!hWnd) return 0;
    ShowWindow(hWnd, nCmdShow);

    // Bucle de mensajes de windows
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int) msg.wParam;
}

// Funcion que maneja los eventos de la ventana
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // Creación de etiquetas y cajas de texto
            CreateWindow("STATIC", "Codigo:", WS_VISIBLE | WS_CHILD, 10, 10, 80, 25, hWnd, NULL, NULL, NULL);
            hEditCodigo = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 10, 100, 25, hWnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "Nombre:", WS_VISIBLE | WS_CHILD, 10, 40, 80, 25, hWnd, NULL, NULL, NULL);
            hEditNombre = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 40, 100, 25, hWnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "Precio:", WS_VISIBLE | WS_CHILD, 10, 70, 80, 25, hWnd, NULL, NULL, NULL);
            hEditPrecio = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 70, 100, 25, hWnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "Cantidad:", WS_VISIBLE | WS_CHILD, 10, 100, 80, 25, hWnd, NULL, NULL, NULL);
            hEditCantidad = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 100, 100, 25, hWnd, NULL, NULL, NULL);
            // Creación de botones
            CreateWindow("BUTTON", "Agregar", WS_VISIBLE | WS_CHILD, 10, 140, 80, 30, hWnd, (HMENU) 1, NULL, NULL);
            CreateWindow("BUTTON", "Mostrar", WS_VISIBLE | WS_CHILD, 100, 140, 80, 30, hWnd, (HMENU) 2, NULL, NULL);
            CreateWindow("BUTTON", "Eliminar", WS_VISIBLE | WS_CHILD, 190, 140, 80, 30, hWnd, (HMENU) 3, NULL, NULL);
            CreateWindow("BUTTON", "Buscar", WS_VISIBLE | WS_CHILD, 280, 140, 80, 30, hWnd, (HMENU) 4, NULL, NULL);
            break;
        }
        case WM_COMMAND:
            // Manejo de eventos de los botones
            switch (LOWORD(wParam)) {
                case 1: // Agregar producto
                    // Obtiene los datos ingresados y los guarda en el inventario
                    char codigo[10], nombre[50], precio[10], cantidad[10];
                    GetWindowText(hEditCodigo, codigo, 10);
                    GetWindowText(hEditNombre, nombre, 50);
                    GetWindowText(hEditPrecio, precio, 10);
                    GetWindowText(hEditCantidad, cantidad, 10);

                    tienda.agregarProducto(atoi(codigo), nombre, atof(precio), atoi(cantidad));
                    mostrarMensaje(hWnd, "Producto agregado correctamente.");
                    break;
                case 2: // Mostrar inventario
                    mostrarInventario(hWnd);
                    break;
                case 3: // Eliminar producto
                    char codigoEliminar[10];
                    GetWindowText(hEditCodigo, codigoEliminar, 10);
                    tienda.eliminarProducto(atoi(codigoEliminar));
                    mostrarMensaje(hWnd, "Producto eliminado correctamente.");
                    
                    break;
                case 4: // Buscar producto
                char codigoBuscar[10];
                GetWindowText(hEditCodigo, codigoBuscar, 10);
                string resultado = tienda.buscarProducto(atoi(codigoBuscar));
                MessageBox(hWnd, resultado.c_str(), "Resultado de busqueda", MB_OK | MB_ICONINFORMATION);
                break;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}
