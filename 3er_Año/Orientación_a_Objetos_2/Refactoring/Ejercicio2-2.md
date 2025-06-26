# Ejercicio 2.2


## Código Original
```java
public class Juego {
    // ......
    public void incrementar(Jugador j) {
        j.puntuacion = j.puntuacion + 100;
    }
    public void decrementar(Jugador j) {
        j.puntuacion = j.puntuacion - 50;
    }
}

public class Jugador {
    public String nombre;
    public String apellido;
    public int puntuacion = 0;
}
```

## Bad Smell 1: Inconsistent naming, de incrementar() y decrementar()

### Refactoring: Rename Method
Consiste en renombrar un método para que tengo un nombre correcto
Mecanica simple:
* Si estás eliminando un parámetro, asegúrate de que no se use en el cuerpo de la función.
* Cambia la declaración del método a la deseada.
* Encuentra todas las referencias a la declaración antigua y actualízalas a la nueva.
* Compilar y testear.

```java
public class Juego {
    // ......
    public void incrementarPuntuación(Jugador j) {
        j.puntuacion = j.puntuacion + 100;
    }
    public void decrementarPuntuación(Jugador j) {
        j.puntuacion = j.puntuacion - 50;
    }
}

public class Jugador {
    public String nombre;
    public String apellido;
    public int puntuacion = 0;
}
```

## Bad Smell 2: Feature Envy de incrementarPuntuación() y decrementarPuntuación() y DataClass en Jugador

### Refactoring: Move Method
Mecánica:
* Examinar los elementos del programa usados por la función en su contexto actual. Decide si algunos deberían moverse también.
Si una función llamada debe moverse, hazlo primero (comienza por la de menor dependencia).
Si una función de alto nivel es la única que llama a subfunciones, puedes:
Hacer inline de las subfunciones.
Mover la función principal.
Volver a extraer las subfunciones en el destino.

* Verificar si la función es un método polimórfico (en lenguajes OO, considera superclases y subclases).

* Copiar la función al contexto destino y ajústala:
Si usa elementos del contexto origen, pásalos como parámetros o una referencia al contexto original.
Renombra la función si es necesario para que encaje mejor en el nuevo contexto.

* Realiza análisis estático (verificación de código).

* Define cómo invocar la función destino desde el contexto origen:
Convierte la función origen en una delegating function.

* Prueba.

* Considera aplicar Inline Function (115) sobre la función origen:

> Las delegating functions pueden mantenerse indefinidamente, pero si los llamadores pueden acceder directamente al destino, elimina el intermediario (remove the middle man).

```java
public class Juego {
    private List<Jugador> jugadores;
    // ......
    public void incrementarPuntuacion (Jugador jugador){
        jugador.incrementarPuntuación();
    }
    
    public void decrementarPuntuacion (Jugador jugador){
        jugador.incrementarPuntuación();
    }

}

public class Jugador {
    public String nombre;
    public String apellido;
    public int puntuacion = 0;

public void incrementarPuntuación() {
    this.puntuacion = this.puntuacion + 100;
}

public void decrementarPuntuación() {
    this.puntuacion = this.puntuacion - 50;
}

}
```


## Bad Smell 3: Inconsistent naming, de incrementarPuntuacion() y decrementarPuntuacion() en Juego

### Refactoring: Rename method
Mecanica simple:
* Si estás eliminando un parámetro, asegúrate de que no se use en el cuerpo de la función.
* Cambia la declaración del método a la deseada.
* Encuentra todas las referencias a la declaración antigua y actualízalas a la nueva.
* Compilar y testear.

```java
public class Juego {
    private List<Jugador> jugadores;
    // ......
    public void incrementarPuntuacionJugador (Jugador jugador){
        jugador.incrementarPuntuación();
    }
    
    public void decrementarPuntuacionJugador (Jugador jugador){
        jugador.incrementarPuntuación();
    }

}

public class Jugador {
    public String nombre;
    public String apellido;
    public int puntuacion = 0;

public void incrementarPuntuación() {
    this.puntuacion = this.puntuacion + 100;
}

public void decrementarPuntuación() {
    this.puntuacion = this.puntuacion - 50;
}

}
```

## Bad Smell 4: Falta constructor en ambas clases
### Refactoring: No tiene un nombre

```java
public class Juego {
    private List<Jugador> jugadores;

    public Juego(){
        this.jugadores = new ArrayList<Jugador> jugadores();
    }

    // ......

    public void incrementarPuntuacionJugador (Jugador jugador){
        jugador.incrementarPuntuación();
    }
    
    public void decrementarPuntuacionJugador (Jugador jugador){
        jugador.incrementarPuntuación();
    }

}

public class Jugador {
    public String nombre;
    public String apellido;
    public int puntuacion;

    public Jugador(String nombre, String apellido){
        this.nombre = nombre;
        this.apellido = apellido;
        this.puntuacion = 0;
    }

public void incrementarPuntuación() {
    this.puntuacion = this.puntuacion + 100;
}

public void decrementarPuntuación() {
    this.puntuacion = this.puntuacion - 50;
}

}
```


