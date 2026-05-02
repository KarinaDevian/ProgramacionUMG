#include <iostream>
#include <pqxx/pqxx>

using namespace std;

// FUNCION: VALIDACION
// valida que el usuario unicamente ingrese numeros enteros
int leerEntero(const string& mensaje)
{
    // variable que guarda el valor ingresado
    int valor;

    // ciclo infinito hasta recibir un numero valido
    while (true)
    {
        cout << mensaje;
        cin >> valor;

        // verifica si el usuario ingreso un dato invalido
        if (cin.fail())
        {
            // limpia el error del buffer
            cin.clear();
            cin.ignore(1000, '\n');

            // imprime mensaje de error
            cout << "Entrada invalida. Ingrese solo numeros." << endl;
        }
        else
        {
            // limpia saltos de linea sobrantes
            cin.ignore(1000, '\n');

            // retorna el valor valido
            return valor;
        }
    }
}

// FUNCION: CONEXION
// realiza la conexion con PostgreSQL
pqxx::connection* conectar()
{
    // crea conexion utilizando host, puerto, base de datos y credenciales
    pqxx::connection* conn = new pqxx::connection(
        "host=localhost port=5432 dbname=universidad user=postgres password=postgres"
    );

    // verifica si la conexion fue exitosa
    if (conn->is_open())
        cout << "Conexion exitosa a: " << conn->dbname() << endl;

    // retorna puntero de conexion
    return conn;
}

// ==================== ALUMNOS ====================

// FUNCION: listarAlumnos
// imprime todos los alumnos con su curso y seccion asignados usando JOIN
void listarAlumnos(pqxx::connection* conn)
{
    // inicia consulta sin modificar datos
    pqxx::nontransaction n(*conn);

    // consulta con LEFT JOIN para incluir alumnos sin curso o seccion asignada
    // COALESCE muestra texto alternativo si el valor es nulo
    pqxx::result r = n.exec(
        "SELECT a.id, a.nombres, a.apellidos, a.carnet, "
        "COALESCE(c.nombre, 'Sin curso') AS curso, "
        "COALESCE(s.nombre, 'Sin seccion') AS seccion "
        "FROM alumnos a "
        "LEFT JOIN alumnos_cursos ac ON a.id = ac.alumno_id "
        "LEFT JOIN cursos c ON ac.curso_id = c.id "
        "LEFT JOIN alumnos_seccion ase ON a.id = ase.alumno_id "
        "LEFT JOIN secciones s ON ase.seccion_id = s.id "
        "ORDER BY a.id"
    );

    cout << "\n--- Lista de Alumnos ---" << endl;

    // verifica si hay registros
    if (r.empty())
    {
        cout << "No hay alumnos registrados." << endl;
        return;
    }

    // imprime encabezados de columnas
    cout << "ID | Nombres | Apellidos | Carnet | Curso | Seccion" << endl;
    cout << "---|---------|-----------|--------|-------|--------" << endl;

    // recorre e imprime cada fila del resultado
    for (auto row : r)
        cout << row["id"].as<int>() << " | "
        << row["nombres"].c_str() << " | "
        << row["apellidos"].c_str() << " | "
        << row["carnet"].c_str() << " | "
        << row["curso"].c_str() << " | "
        << row["seccion"].c_str() << endl;
}

// FUNCION: grabarAlumno
// solicita datos al usuario e inserta un nuevo alumno en la base de datos
void grabarAlumno(pqxx::connection* conn)
{

    cout << "\n--- Grabar Nuevo Alumnos ---" << endl;

    // variables para almacenar datos ingresados
    string nombres, apellidos, carnet;
    cout << "Nombres: ";
    getline(cin, nombres);
    cout << "Apellidos: ";
    getline(cin, apellidos);
    cout << "Carnet: ";
    getline(cin, carnet);

    // inicia transaccion para modificar datos
    pqxx::work w(*conn);

    // ejecuta INSERT con los datos ingresados
    // w.quote() protege contra inyeccion SQL
    w.exec("INSERT INTO alumnos (nombres, apellidos, carnet) VALUES ("
        + w.quote(nombres) + ", "
        + w.quote(apellidos) + ", "
        + w.quote(carnet) + ")");

    // confirma la transaccion
    w.commit();
    cout << "Alumno grabado correctamente." << endl;
}

// FUNCION: modificarAlumno
// muestra lista, solicita ID y actualiza datos del alumno seleccionado
void modificarAlumno(pqxx::connection* conn)
{

    // muestra lista para que el usuario identifique el ID
    listarAlumnos(conn);
    string nombres, apellidos, carnet;

    cout << "\n--- Modificar Alumnos ---" << endl;

    // solicita ID con validacion numerica
    int id = leerEntero("ID del alumno a modificar: ");
    cout << "Nuevos nombres: ";
    getline(cin, nombres);
    cout << "Nuevos apellidos: ";
    getline(cin, apellidos);
    cout << "Nuevo carnet: ";
    getline(cin, carnet);

    // inicia transaccion y ejecuta UPDATE
    pqxx::work w(*conn);
    pqxx::result r = w.exec("UPDATE alumnos SET nombres = "
        + w.quote(nombres) + ", apellidos = "
        + w.quote(apellidos) + ", carnet = "
        + w.quote(carnet)
        + " WHERE id = " + to_string(id));
    w.commit();

    // verifica si se modifico algun registro
    if (r.affected_rows() > 0)
        cout << "Alumno modificado correctamente." << endl;
    else
        cout << "No se encontro un alumno con ese ID." << endl;
}

// FUNCION: eliminarAlumno
// elimina alumno y sus relaciones con cursos y seccion
void eliminarAlumno(pqxx::connection* conn)
{

    // muestra lista para que el usuario identifique el ID
    listarAlumnos(conn);

    cout << "\n--- Eliminar Alumnos ---" << endl;

    int id = leerEntero("ID del alumno a eliminar: ");

    // inicia transaccion
    pqxx::work w(*conn);

    // primero elimina relaciones para no violar restricciones de clave foranea
    w.exec("DELETE FROM alumnos_cursos WHERE alumno_id = " + to_string(id));
    w.exec("DELETE FROM alumnos_seccion WHERE alumno_id = " + to_string(id));

    // luego elimina el alumno
    pqxx::result r = w.exec("DELETE FROM alumnos WHERE id = " + to_string(id));
    w.commit();

    // verifica si se elimino algun registro
    if (r.affected_rows() > 0)
        cout << "Alumno eliminado correctamente." << endl;
    else
        cout << "No se encontro un alumno con ese ID." << endl;
}

// FUNCION: menuAlumnos
// muestra submenu de alumnos y redirige a la funcion correspondiente
void menuAlumnos(pqxx::connection* conn)
{
    int opcion;
    do {
        cout << "\n===== MODULO ALUMNOS =====" << endl;
        cout << "1. Listar" << endl;
        cout << "2. Grabar" << endl;
        cout << "3. Modificar" << endl;
        cout << "4. Eliminar" << endl;
        cout << "5. Regresar" << endl;

        // lee opcion con validacion numerica
        opcion = leerEntero("Opcion: ");

        // ejecuta la funcion correspondiente segun opcion
        switch (opcion)
        {
        case 1: listarAlumnos(conn); break;
        case 2: grabarAlumno(conn); break;
        case 3: modificarAlumno(conn); break;
        case 4: eliminarAlumno(conn); break;
        case 5: cout << "Regresando..." << endl; break;
        default: cout << "Opcion invalida." << endl;
        }
    } while (opcion != 5); // repite hasta que el usuario elija regresar
}

// ==================== CURSOS ====================

// FUNCION: listarCursos
// imprime todos los cursos registrados
void listarCursos(pqxx::connection* conn)
{
    // inicia consulta sin modificar datos
    pqxx::nontransaction n(*conn);
    pqxx::result r = n.exec("SELECT id, nombre, codigo FROM cursos ORDER BY id");

    cout << "\n--- Lista de Cursos ---" << endl;

    // verifica si hay registros
    if (r.empty())
    {
        cout << "No hay cursos registrados." << endl;
        return;
    }

    // imprime encabezados y filas
    cout << "ID | Nombre | Codigo" << endl;
    cout << "---|--------|-------" << endl;
    for (auto row : r)
        cout << row["id"].as<int>() << " | "
        << row["nombre"].c_str() << " | "
        << row["codigo"].c_str() << endl;
}

// FUNCION: grabarCurso
// solicita datos al usuario e inserta un nuevo curso
void grabarCurso(pqxx::connection* conn)
{

    cout << "\n--- Grabar Nuevo Curso ---" << endl;

    string nombre, codigo;
    cout << "Nombre del curso: ";
    getline(cin, nombre);
    cout << "Codigo del curso: ";
    getline(cin, codigo);

    // inicia transaccion y ejecuta INSERT
    pqxx::work w(*conn);
    w.exec("INSERT INTO cursos (nombre, codigo) VALUES ("
        + w.quote(nombre) + ", "
        + w.quote(codigo) + ")");
    w.commit();
    cout << "Curso grabado correctamente." << endl;
}

// FUNCION: modificarCurso
// muestra lista, solicita ID y actualiza datos del curso seleccionado
void modificarCurso(pqxx::connection* conn)
{

    listarCursos(conn);
    string nombre, codigo;

    cout << "\n--- Modificar Curso ---" << endl;

    int id = leerEntero("ID del curso a modificar: ");
    cout << "Nuevo nombre: ";
    getline(cin, nombre);
    cout << "Nuevo codigo: ";
    getline(cin, codigo);

    // inicia transaccion y ejecuta UPDATE
    pqxx::work w(*conn);
    pqxx::result r = w.exec("UPDATE cursos SET nombre = "
        + w.quote(nombre) + ", codigo = "
        + w.quote(codigo)
        + " WHERE id = " + to_string(id));
    w.commit();

    // verifica si se modifico algun registro
    if (r.affected_rows() > 0)
        cout << "Curso modificado correctamente." << endl;
    else
        cout << "No se encontro un curso con ese ID." << endl;
}

// FUNCION: eliminarCurso
// valida que el curso no este asignado antes de eliminarlo
void eliminarCurso(pqxx::connection* conn)
{

    listarCursos(conn);

    cout << "\n--- Eliminar Curso ---" << endl;

    int id = leerEntero("ID del curso a eliminar: ");

    // inicia transaccion
    pqxx::work w(*conn);

    // verifica si el curso esta asignado a algun alumno
    pqxx::result check = w.exec(
        "SELECT COUNT(*) FROM alumnos_cursos WHERE curso_id = " + to_string(id));

    // si tiene alumnos asignados, no permite eliminar
    if (check[0][0].as<int>() > 0)
    {
        cout << "No se puede eliminar: el curso esta asignado a alumnos." << endl;
        return;
    }

    // si no tiene asignaciones, procede a eliminar
    pqxx::result r = w.exec("DELETE FROM cursos WHERE id = " + to_string(id));
    w.commit();

    if (r.affected_rows() > 0)
        cout << "Curso eliminado correctamente." << endl;
    else
        cout << "No se encontro un curso con ese ID." << endl;
}

// FUNCION: menuCursos
// muestra submenu de cursos y redirige a la funcion correspondiente
void menuCursos(pqxx::connection* conn)
{
    int opcion;
    do {
        cout << "\n===== MODULO CURSOS =====" << endl;
        cout << "1. Listar" << endl;
        cout << "2. Grabar" << endl;
        cout << "3. Modificar" << endl;
        cout << "4. Eliminar" << endl;
        cout << "5. Regresar" << endl;
        opcion = leerEntero("Opcion: ");

        switch (opcion)
        {
        case 1: listarCursos(conn); break;
        case 2: grabarCurso(conn); break;
        case 3: modificarCurso(conn); break;
        case 4: eliminarCurso(conn); break;
        case 5: cout << "Regresando..." << endl; break;
        default: cout << "Opcion invalida." << endl;
        }
    } while (opcion != 5);
}

// ==================== SECCIONES ====================

// FUNCION: listarSecciones
// imprime todas las secciones registradas
void listarSecciones(pqxx::connection* conn)
{
    // inicia consulta sin modificar datos
    pqxx::nontransaction n(*conn);
    pqxx::result r = n.exec("SELECT id, nombre, jornada FROM secciones ORDER BY id");

    cout << "\n--- Lista de Secciones ---" << endl;

    // verifica si hay registros
    if (r.empty())
    {
        cout << "No hay secciones registradas." << endl;
        return;
    }

    // imprime encabezados y filas
    cout << "ID | Nombre | Jornada" << endl;
    cout << "---|--------|--------" << endl;
    for (auto row : r)
        cout << row["id"].as<int>() << " | "
        << row["nombre"].c_str() << " | "
        << row["jornada"].c_str() << endl;
}

// FUNCION: grabarSeccion
// solicita datos al usuario e inserta una nueva seccion
void grabarSeccion(pqxx::connection* conn)
{

    cout << "\n--- Grabar Nueva Seccion ---" << endl;

    string nombre, jornada;
    cout << "Nombre de la seccion: ";
    getline(cin, nombre);
    cout << "Jornada (Matutina/Vespertina/Nocturna): ";
    getline(cin, jornada);

    // inicia transaccion y ejecuta INSERT
    pqxx::work w(*conn);
    w.exec("INSERT INTO secciones (nombre, jornada) VALUES ("
        + w.quote(nombre) + ", "
        + w.quote(jornada) + ")");
    w.commit();
    cout << "Seccion grabada correctamente." << endl;
}

// FUNCION: modificarSeccion
// muestra lista, solicita ID y actualiza datos de la seccion seleccionada
void modificarSeccion(pqxx::connection* conn)
{

    listarSecciones(conn);
    string nombre, jornada;

    cout << "\n--- Modificar Seccion ---" << endl;

    int id = leerEntero("ID de la seccion a modificar: ");

    cout << "Nuevo nombre: ";
    getline(cin, nombre);
    cout << "Nueva jornada: ";
    getline(cin, jornada);

    // inicia transaccion y ejecuta UPDATE
    pqxx::work w(*conn);
    pqxx::result r = w.exec("UPDATE secciones SET nombre = "
        + w.quote(nombre) + ", jornada = "
        + w.quote(jornada)
        + " WHERE id = " + to_string(id));
    w.commit();

    if (r.affected_rows() > 0)
        cout << "Seccion modificada correctamente." << endl;
    else
        cout << "No se encontro una seccion con ese ID." << endl;
}

// FUNCION: eliminarSeccion
// valida que la seccion no tenga alumnos asignados antes de eliminarla
void eliminarSeccion(pqxx::connection* conn)
{

    listarSecciones(conn);

    cout << "\n--- Eliminar Seccion ---" << endl;

    int id = leerEntero("ID de la seccion a eliminar: ");

    // inicia transaccion
    pqxx::work w(*conn);

    // verifica si la seccion tiene alumnos asignados
    pqxx::result check = w.exec(
        "SELECT COUNT(*) FROM alumnos_seccion WHERE seccion_id = " + to_string(id));

    // si tiene alumnos, no permite eliminar
    if (check[0][0].as<int>() > 0)
    {
        cout << "No se puede eliminar: la seccion tiene alumnos asignados." << endl;
        return;
    }

    // si no tiene asignaciones, procede a eliminar
    pqxx::result r = w.exec("DELETE FROM secciones WHERE id = " + to_string(id));
    w.commit();

    if (r.affected_rows() > 0)
        cout << "Seccion eliminada correctamente." << endl;
    else
        cout << "No se encontro una seccion con ese ID." << endl;
}

// FUNCION: menuSecciones
// muestra submenu de secciones y redirige a la funcion correspondiente
void menuSecciones(pqxx::connection* conn)
{
    int opcion;
    do {
        cout << "\n===== MODULO SECCIONES =====" << endl;
        cout << "1. Listar" << endl;
        cout << "2. Grabar" << endl;
        cout << "3. Modificar" << endl;
        cout << "4. Eliminar" << endl;
        cout << "5. Regresar" << endl;
        opcion = leerEntero("Opcion: ");

        switch (opcion)
        {
        case 1: listarSecciones(conn); break;
        case 2: grabarSeccion(conn); break;
        case 3: modificarSeccion(conn); break;
        case 4: eliminarSeccion(conn); break;
        case 5: cout << "Regresando..." << endl; break;
        default: cout << "Opcion invalida." << endl;
        }
    } while (opcion != 5);
}

// ==================== ALUMNOS - CURSOS ====================

// FUNCION: listarAlumnosCursos
// imprime todas las asignaciones de alumnos con sus cursos
void listarAlumnosCursos(pqxx::connection* conn)
{
    // inicia consulta sin modificar datos
    pqxx::nontransaction n(*conn);

    // JOIN para mostrar nombres en lugar de solo IDs
    pqxx::result r = n.exec(
        "SELECT ac.id, a.nombres, a.apellidos, c.nombre AS curso "
        "FROM alumnos_cursos ac "
        "JOIN alumnos a ON ac.alumno_id = a.id "
        "JOIN cursos c ON ac.curso_id = c.id "
        "ORDER BY ac.id"
    );

    cout << "\n--- Asignaciones Alumno - Curso ---" << endl;

    // verifica si hay registros
    if (r.empty())
    {
        cout << "No hay asignaciones registradas." << endl;
        return;
    }

    // imprime encabezados y filas
    cout << "ID | Alumno | Curso" << endl;
    cout << "---|--------|------" << endl;
    for (auto row : r)
        cout << row["id"].as<int>() << " | "
        << row["nombres"].c_str() << " "
        << row["apellidos"].c_str() << " | "
        << row["curso"].c_str() << endl;
}

// FUNCION: asignarCursoAlumno
// asigna un curso a un alumno validando que no este duplicado
void asignarCursoAlumno(pqxx::connection* conn)
{

    // muestra alumnos y cursos para facilitar la seleccion
    listarAlumnos(conn);

    cout << "\n--- Asignar Curso ---" << endl;

    int alumno_id = leerEntero("ID del alumno: ");

    listarCursos(conn);
    int curso_id = leerEntero("ID del curso: ");

    // inicia transaccion
    pqxx::work w(*conn);

    // verifica si la asignacion ya existe para evitar duplicados
    pqxx::result check = w.exec(
        "SELECT COUNT(*) FROM alumnos_cursos WHERE alumno_id = "
        + to_string(alumno_id) + " AND curso_id = " + to_string(curso_id));

    // si ya existe, no permite asignar de nuevo
    if (check[0][0].as<int>() > 0)
    {
        cout << "El alumno ya tiene ese curso asignado." << endl;
        return;
    }

    // inserta la nueva asignacion
    w.exec("INSERT INTO alumnos_cursos (alumno_id, curso_id) VALUES ("
        + to_string(alumno_id) + ", "
        + to_string(curso_id) + ")");
    w.commit();
    cout << "Curso asignado correctamente." << endl;
}

// FUNCION: eliminarAsignacionCurso
// elimina una asignacion alumno-curso por su ID
void eliminarAsignacionCurso(pqxx::connection* conn)
{

    // muestra asignaciones para que el usuario identifique el ID
    listarAlumnosCursos(conn);

    cout << "\n--- Eliminar Asignacion ---" << endl;

    int id = leerEntero("ID de la asignacion a eliminar: ");

    // inicia transaccion y ejecuta DELETE
    pqxx::work w(*conn);
    pqxx::result r = w.exec(
        "DELETE FROM alumnos_cursos WHERE id = " + to_string(id));
    w.commit();

    if (r.affected_rows() > 0)
        cout << "Asignacion eliminada correctamente." << endl;
    else
        cout << "No se encontro una asignacion con ese ID." << endl;
}

// FUNCION: menuAlumnosCursos
// muestra submenu de asignaciones alumno-curso
void menuAlumnosCursos(pqxx::connection* conn)
{
    int opcion;
    do {
        cout << "\n===== MODULO ALUMNOS - CURSOS =====" << endl;
        cout << "1. Listar" << endl;
        cout << "2. Asignar" << endl;
        cout << "3. Eliminar asignacion" << endl;
        cout << "4. Regresar" << endl;
        opcion = leerEntero("Opcion: ");

        switch (opcion)
        {
        case 1: listarAlumnosCursos(conn); break;
        case 2: asignarCursoAlumno(conn); break;
        case 3: eliminarAsignacionCurso(conn); break;
        case 4: cout << "Regresando..." << endl; break;
        default: cout << "Opcion invalida." << endl;
        }
    } while (opcion != 4);
}

// ==================== ALUMNOS - SECCION ====================

// FUNCION: listarAlumnosSeccion
// imprime todas las asignaciones de alumnos con sus secciones
void listarAlumnosSeccion(pqxx::connection* conn)
{
    // inicia consulta sin modificar datos
    pqxx::nontransaction n(*conn);

    // JOIN para mostrar nombres en lugar de solo IDs
    pqxx::result r = n.exec(
        "SELECT ase.id, a.nombres, a.apellidos, s.nombre AS seccion "
        "FROM alumnos_seccion ase "
        "JOIN alumnos a ON ase.alumno_id = a.id "
        "JOIN secciones s ON ase.seccion_id = s.id "
        "ORDER BY ase.id"
    );

    cout << "\n--- Asignaciones Alumno - Seccion ---" << endl;

    // verifica si hay registros
    if (r.empty())
    {
        cout << "No hay asignaciones registradas." << endl;
        return;
    }

    // imprime encabezados y filas
    cout << "ID | Alumno | Seccion" << endl;
    cout << "---|--------|--------" << endl;
    for (auto row : r)
        cout << row["id"].as<int>() << " | "
        << row["nombres"].c_str() << " "
        << row["apellidos"].c_str() << " | "
        << row["seccion"].c_str() << endl;
}

// FUNCION: asignarSeccionAlumno
// asigna una seccion a un alumno, validando que no tenga una ya asignada
void asignarSeccionAlumno(pqxx::connection* conn)
{

    // muestra alumnos para facilitar la seleccion
    listarAlumnos(conn);

    cout << "\n--- Asignar Seccion ---" << endl;

    int alumno_id = leerEntero("ID del alumno: ");

    // inicia transaccion para verificar asignacion existente
    pqxx::work w(*conn);

    // verifica si el alumno ya tiene una seccion asignada
    pqxx::result check = w.exec(
        "SELECT COUNT(*) FROM alumnos_seccion WHERE alumno_id = "
        + to_string(alumno_id));

    // cada alumno puede tener como maximo una seccion activa
    if (check[0][0].as<int>() > 0)
    {
        cout << "El alumno ya tiene una seccion asignada. Use Modificar." << endl;
        return;
    }

    // cierra la primera transaccion antes de listar secciones
    w.commit();

    // muestra secciones para facilitar la seleccion
    listarSecciones(conn);
    int seccion_id = leerEntero("ID de la seccion: ");

    // inicia nueva transaccion para insertar
    pqxx::work w2(*conn);
    w2.exec("INSERT INTO alumnos_seccion (alumno_id, seccion_id) VALUES ("
        + to_string(alumno_id) + ", "
        + to_string(seccion_id) + ")");
    w2.commit();
    cout << "Seccion asignada correctamente." << endl;
}

// FUNCION: modificarSeccionAlumno
// cambia la seccion asignada a un alumno
void modificarSeccionAlumno(pqxx::connection* conn)
{

    // muestra asignaciones actuales para identificar el ID
    listarAlumnosSeccion(conn);

    cout << "\n--- Modificar Asignacion ---" << endl;

    int id = leerEntero("ID de la asignacion a modificar: ");

    // muestra secciones disponibles
    listarSecciones(conn);
    int seccion_id = leerEntero("ID de la nueva seccion: ");

    // inicia transaccion y ejecuta UPDATE
    pqxx::work w(*conn);
    pqxx::result r = w.exec(
        "UPDATE alumnos_seccion SET seccion_id = " + to_string(seccion_id)
        + " WHERE id = " + to_string(id));
    w.commit();

    if (r.affected_rows() > 0)
        cout << "Seccion modificada correctamente." << endl;
    else
        cout << "No se encontro una asignacion con ese ID." << endl;
}

// FUNCION: eliminarAsignacionSeccion
// elimina una asignacion alumno-seccion por su ID
void eliminarAsignacionSeccion(pqxx::connection* conn)
{

    // muestra asignaciones para que el usuario identifique el ID
    listarAlumnosSeccion(conn);

    cout << "\n--- Eliminar Asignacion ---" << endl;

    int id = leerEntero("ID de la asignacion a eliminar: ");

    // inicia transaccion y ejecuta DELETE
    pqxx::work w(*conn);
    pqxx::result r = w.exec(
        "DELETE FROM alumnos_seccion WHERE id = " + to_string(id));
    w.commit();

    if (r.affected_rows() > 0)
        cout << "Asignacion eliminada correctamente." << endl;
    else
        cout << "No se encontro una asignacion con ese ID." << endl;
}

// FUNCION: menuAlumnosSeccion
// muestra submenu de asignaciones alumno-seccion
void menuAlumnosSeccion(pqxx::connection* conn)
{
    int opcion;
    do {
        cout << "\n===== MODULO ALUMNOS - SECCION =====" << endl;
        cout << "1. Listar" << endl;
        cout << "2. Asignar" << endl;
        cout << "3. Modificar asignacion" << endl;
        cout << "4. Eliminar asignacion" << endl;
        cout << "5. Regresar" << endl;
        opcion = leerEntero("Opcion: ");

        switch (opcion)
        {
        case 1: listarAlumnosSeccion(conn); break;
        case 2: asignarSeccionAlumno(conn); break;
        case 3: modificarSeccionAlumno(conn); break;
        case 4: eliminarAsignacionSeccion(conn); break;
        case 5: cout << "Regresando..." << endl; break;
        default: cout << "Opcion invalida." << endl;
        }
    } while (opcion != 5);
}

// ==================== MENU PRINCIPAL ====================

// FUNCION: mostrarMenuPrincipal
// muestra el menu principal y redirige al modulo seleccionado
void mostrarMenuPrincipal(pqxx::connection* conn)
{
    int opcion;
    do {
        cout << "\n===== SISTEMA UNIVERSIDAD =====" << endl;
        cout << "1. Alumnos" << endl;
        cout << "2. Cursos" << endl;
        cout << "3. Secciones" << endl;
        cout << "4. Alumnos - Cursos" << endl;
        cout << "5. Alumnos - Seccion" << endl;
        cout << "6. Salir" << endl;

        // lee opcion con validacion numerica
        opcion = leerEntero("Opcion: ");

        // redirige al modulo correspondiente
        switch (opcion)
        {
        case 1: menuAlumnos(conn); break;
        case 2: menuCursos(conn); break;
        case 3: menuSecciones(conn); break;
        case 4: menuAlumnosCursos(conn); break;
        case 5: menuAlumnosSeccion(conn); break;
        case 6: cout << "Cerrando sistema..." << endl; break;
        default: cout << "Opcion invalida." << endl;
        }
    } while (opcion != 6); // repite hasta que el usuario elija salir
}

// ==================== MAIN ====================

// FUNCION: main
// punto de entrada del programa
// establece conexion, ejecuta el menu y cierra la conexion al salir
int main()
{
    try
    {
        // establece conexion con la base de datos
        pqxx::connection* conn = conectar();

        // inicia el menu principal pasando la conexion
        mostrarMenuPrincipal(conn);

        // cierra la conexion correctamente antes de salir
        conn->close();

        // libera la memoria del puntero
        delete conn;
    }
    catch (const std::exception& e)
    {
        // captura y muestra cualquier error de conexion o ejecucion
        cout << "Error: " << e.what() << endl;
    }
    return 0;
}