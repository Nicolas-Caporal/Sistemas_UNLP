# Ejercicio 2.3 - Publicaciones


## Código Original
```java
/**
* Retorna los últimos N posts que no pertenecen al usuario user
*/
public List<Post> ultimosPosts(Usuario user, int cantidad) {
        
    List<Post> postsOtrosUsuarios = new ArrayList<Post>();
    for (Post post : this.posts) {
        if (!post.getUsuario().equals(user)) {
            postsOtrosUsuarios.add(post);
        }
    }
        
   // ordena los posts por fecha
   for (int i = 0; i < postsOtrosUsuarios.size(); i++) {
       int masNuevo = i;
       for(int j= i +1; j < postsOtrosUsuarios.size(); j++) {
           if (postsOtrosUsuarios.get(j).getFecha().isAfter(
     postsOtrosUsuarios.get(masNuevo).getFecha())) {
              masNuevo = j;
           }    
       }
      Post unPost = postsOtrosUsuarios.set(i,postsOtrosUsuarios.get(masNuevo));
      postsOtrosUsuarios.set(masNuevo, unPost);    
   }
        
    List<Post> ultimosPosts = new ArrayList<Post>();
    int index = 0;
    Iterator<Post> postIterator = postsOtrosUsuarios.iterator();
    while (postIterator.hasNext() &&  index < cantidad) {
        ultimosPosts.add(postIterator.next());
    }
    return ultimosPosts;
}

```

## Bad Smell 1: Long method en ultimosPosts

### Refactoring: Extract method
Mecánica
* Crear una nueva función nombrada por su intención (¿qué hace?, no ¿cómo lo hace?).
Extraer incluso código simple (ej., una llamada) si el nombre revela mejor su propósito.
Si no se encuentra un nombre significativo, evitar la extracción.
Permitir ajustar el nombre después; si no ayuda, volver a inlinear.

* Si el lenguaje soporta funciones anidadas, definir la función extraída dentro de la original para minimizar variables fuera de alcance.

* Copiar el fragmento a la nueva función.

* Identificar variables locales usadas en el fragmento:
Pasar como parámetros las variables locales o parámetros de la función original.
Si una variable solo se usa en el fragmento pero se declara fuera, mover su declaración adentro.

* Manejar variables asignadas con cuidado:
Si es una sola, tratar el fragmento como una consulta (query) y asignar el resultado.
Si hay muchas asignaciones, reconsiderar la extracción (usar Split Variable (240) o Replace Temp with Query (178) antes).

* Compilar para verificar errores (si el lenguaje lo permite).

* Reemplazar el fragmento original con la llamada a la nueva función.

* Probar.

* Buscar duplicados en el código y reemplazarlos con llamadas a la nueva función (Replace Inline Code with Function Call (222)).
Usar herramientas de refactoring o búsquedas manuales para identificar repeticiones.


```java

private List<Posts> obtenerPostsOtrosUsuarios(Usuario user){
    List<Post> postsOtrosUsuarios = new ArrayList<Post>();
    for (Post post : this.posts) {
        if (!post.getUsuario().equals(user)) {
            postsOtrosUsuarios.add(post);
        }
    }
    return postsOtrosUsuarios;
}

/**
* Retorna los últimos N posts que no pertenecen al usuario user
*/
public List<Post> ultimosPosts(Usuario user, int cantidad) {
    
    List<Post> postsOtrosUsuarios = obtenerPostsOtrosUsuarios(user);

        
   // ordena los posts por fecha
   for (int i = 0; i < postsOtrosUsuarios.size(); i++) {
       int masNuevo = i;
       for(int j= i +1; j < postsOtrosUsuarios.size(); j++) {
           if (postsOtrosUsuarios.get(j).getFecha().isAfter(
     postsOtrosUsuarios.get(masNuevo).getFecha())) {
              masNuevo = j;
           }    
       }
      Post unPost = postsOtrosUsuarios.set(i,postsOtrosUsuarios.get(masNuevo));
      postsOtrosUsuarios.set(masNuevo, unPost);    
   }
        
    List<Post> ultimosPosts = new ArrayList<Post>();
    int index = 0;
    Iterator<Post> postIterator = postsOtrosUsuarios.iterator();
    while (postIterator.hasNext() &&  index < cantidad) {
        ultimosPosts.add(postIterator.next());
    }
    return ultimosPosts;
}
```


## Bad Smell 2: Long method en ultimosPosts

### Refactoring: Extract method
Mecánica
* Crear una nueva función nombrada por su intención (¿qué hace?, no ¿cómo lo hace?).
Extraer incluso código simple (ej., una llamada) si el nombre revela mejor su propósito.
Si no se encuentra un nombre significativo, evitar la extracción.
Permitir ajustar el nombre después; si no ayuda, volver a inlinear.

* Si el lenguaje soporta funciones anidadas, definir la función extraída dentro de la original para minimizar variables fuera de alcance.

* Copiar el fragmento a la nueva función.

* Identificar variables locales usadas en el fragmento:
Pasar como parámetros las variables locales o parámetros de la función original.
Si una variable solo se usa en el fragmento pero se declara fuera, mover su declaración adentro.

* Manejar variables asignadas con cuidado:
Si es una sola, tratar el fragmento como una consulta (query) y asignar el resultado.
Si hay muchas asignaciones, reconsiderar la extracción (usar Split Variable (240) o Replace Temp with Query (178) antes).

* Compilar para verificar errores (si el lenguaje lo permite).

* Reemplazar el fragmento original con la llamada a la nueva función.

* Probar.

* Buscar duplicados en el código y reemplazarlos con llamadas a la nueva función (Replace Inline Code with Function Call (222)).
Usar herramientas de refactoring o búsquedas manuales para identificar repeticiones.


```java

private List<Posts> obtenerPostsOtrosUsuarios(Usuario user){
    List<Post> postsOtrosUsuarios = new ArrayList<Post>();
    for (Post post : this.posts) {
        if (!post.getUsuario().equals(user)) {
            postsOtrosUsuarios.add(post);
        }
    }
    return postsOtrosUsuarios;
}

private void ordenarPostsPorFechaDescendente(List<Post> posts) {
    for (int i = 0; i < posts.size(); i++) {
        int masNuevo = i;
        for(int j = i + 1; j < posts.size(); j++) {
            if (posts.get(j).getFecha().isAfter(
                    posts.get(masNuevo).getFecha())) {
                masNuevo = j;
            }
        }
        Post unPost = posts.set(i, posts.get(masNuevo));
        posts.set(masNuevo, unPost);
    }
}

/**
* Retorna los últimos N posts que no pertenecen al usuario user
*/
public List<Post> ultimosPosts(Usuario user, int cantidad) {
    
    List<Post> postsOtrosUsuarios = obtenerPostsOtrosUsuarios(user);
    
    ordenarPostsPorFechaDescendente(postsOtrosUsuarios);
    
    List<Post> ultimosPosts = new ArrayList<Post>();
    int index = 0;
    Iterator<Post> postIterator = postsOtrosUsuarios.iterator();
    while (postIterator.hasNext() && index < cantidad) {
        ultimosPosts.add(postIterator.next());
        index++;
    }
    return ultimosPosts;
}
```

## Bad Smell 3: Reinventar la rueda en ultimosPosts

## Refactoring: Replace Loop with Pipeline
Mecanica:
* Crear una nueva variable para la colección usada en el bucle.
* Copiar esa colección desde una variable existente, si hace falta.
* Empezar desde arriba e ir reemplazando cada parte del comportamiento del bucle con una operación del pipeline.
* Probar después de cada cambio.
* Una vez que todo el comportamiento esté en el pipeline, eliminar el bucle.
* Si el bucle usaba un acumulador, asignar el resultado del pipeline a esa variable.


```java
private List<Posts> obtenerPostsOtrosUsuarios(Usuario user){
    List<Post> postsOtrosUsuarios = new ArrayList<Post>();
    for (Post post : this.posts) {
        if (!post.getUsuario().equals(user)) {
            postsOtrosUsuarios.add(post);
        }
    }
    return postsOtrosUsuarios;
}

private void ordenarPostsPorFechaDescendente(List<Post> posts) {
    for (int i = 0; i < posts.size(); i++) {
        int masNuevo = i;
        for(int j = i + 1; j < posts.size(); j++) {
            if (posts.get(j).getFecha().isAfter(
                    posts.get(masNuevo).getFecha())) {
                masNuevo = j;
            }
        }
        Post unPost = posts.set(i, posts.get(masNuevo));
        posts.set(masNuevo, unPost);
    }
}

/**
* Retorna los últimos N posts que no pertenecen al usuario user
*/
public List<Post> ultimosPosts(Usuario user, int cantidad) {
    List<Post> postsOtrosUsuarios = obtenerPostsOtrosUsuarios(user);
    ordenarPostsPorFechaDescendente(postsOtrosUsuarios);
    return postsOtrosUsuarios.stream().limit(cantidad).collect(Collectors.toList());
}
```

## Bad Smell 4: Bad name en ultimosPosts

## Refactoring: Rename method
Consiste en renombrar un método para que tengo un nombre correcto
Mecanica simple:
* Si estás eliminando un parámetro, asegúrate de que no se use en el cuerpo de la función.
* Cambia la declaración del método a la deseada.
* Encuentra todas las referencias a la declaración antigua y actualízalas a la nueva.
* Compilar y testear.



```java
private List<Posts> obtenerPostsOtrosUsuarios(Usuario user){
    List<Post> postsOtrosUsuarios = new ArrayList<Post>();
    for (Post post : this.posts) {
        if (!post.getUsuario().equals(user)) {
            postsOtrosUsuarios.add(post);
        }
    }
    return postsOtrosUsuarios;
}

private void ordenarPostsPorFechaDescendente(List<Post> posts) {
    for (int i = 0; i < posts.size(); i++) {
        int masNuevo = i;
        for(int j = i + 1; j < posts.size(); j++) {
            if (posts.get(j).getFecha().isAfter(
                    posts.get(masNuevo).getFecha())) {
                masNuevo = j;
            }
        }
        Post unPost = posts.set(i, posts.get(masNuevo));
        posts.set(masNuevo, unPost);
    }
}

public List<Post> ultimosPostsDeOtrosUsuarios(Usuario user, int cantidad) {
    List<Post> postsOtrosUsuarios = obtenerPostsOtrosUsuarios(user);
    ordenarPostsPorFechaDescendente(postsOtrosUsuarios);
    return postsOtrosUsuarios.stream().limit(cantidad).collect(Collectors.toList());
}
```


## Bad Smell 5: Reinventar la rueda en obtenerPostsOtrosUsuarios

## Refactoring: Replace Loop with Pipeline
Mecanica:
* Crear una nueva variable para la colección usada en el bucle.
* Copiar esa colección desde una variable existente, si hace falta.
* Empezar desde arriba e ir reemplazando cada parte del comportamiento del bucle con una operación del pipeline.
* Probar después de cada cambio.
* Una vez que todo el comportamiento esté en el pipeline, eliminar el bucle.
* Si el bucle usaba un acumulador, asignar el resultado del pipeline a esa variable.


```java
private List<Post> obtenerPostsOtrosUsuarios(Usuario user) {
    return this.posts.stream()
        .filter(post -> !post.getUsuario().equals(user))
        .collect(Collectors.toList());
}

private void ordenarPostsPorFechaDescendente(List<Post> posts) {
    for (int i = 0; i < posts.size(); i++) {
        int masNuevo = i;
        for(int j = i + 1; j < posts.size(); j++) {
            if (posts.get(j).getFecha().isAfter(
                    posts.get(masNuevo).getFecha())) {
                masNuevo = j;
            }
        }
        Post unPost = posts.set(i, posts.get(masNuevo));
        posts.set(masNuevo, unPost);
    }
}

/**
* Retorna los últimos N posts que no pertenecen al usuario user
*/
public List<Post> ultimosPosts(Usuario user, int cantidad) {
    List<Post> postsOtrosUsuarios = obtenerPostsOtrosUsuarios(user);
    ordenarPostsPorFechaDescendente(postsOtrosUsuarios);
    return postsOtrosUsuarios.stream().limit(cantidad).collect(Collectors.toList());
}
```


## Bad Smell 5: Reinventar la rueda en ordenarPostsPorFechaDescendente

## Refactoring: Replace Loop with Pipeline
Mecanica:
* Crear una nueva variable para la colección usada en el bucle.
* Copiar esa colección desde una variable existente, si hace falta.
* Empezar desde arriba e ir reemplazando cada parte del comportamiento del bucle con una operación del pipeline.
* Probar después de cada cambio.
* Una vez que todo el comportamiento esté en el pipeline, eliminar el bucle.
* Si el bucle usaba un acumulador, asignar el resultado del pipeline a esa variable.


```java
private List<Post> obtenerPostsOtrosUsuarios(Usuario user) {
    return this.posts.stream()
        .filter(post -> !post.getUsuario().equals(user))
        .collect(Collectors.toList());
}

private void ordenarPostsPorFechaDescendente(List<Post> posts) {
    List<Post> sorted = posts.stream()
        .sorted(Comparator.comparing(Post::getFecha).reversed())
        .collect(Collectors.toList());
    
    posts.clear();
    posts.addAll(sorted);
}

public List<Post> ultimosPosts(Usuario user, int cantidad) {
    List<Post> postsOtrosUsuarios = obtenerPostsOtrosUsuarios(user);
    ordenarPostsPorFechaDescendente(postsOtrosUsuarios);
    return postsOtrosUsuarios.stream().limit(cantidad).collect(Collectors.toList());
}
```

## Bad Smell 6: Feature Envy del equals en obtenerPostsOtrosUsuarios

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
private List<Post> obtenerPostsOtrosUsuarios(Usuario user) {
    return this.posts.stream()
        .filter(post -> !post.esDelUsuario(user))
        .collect(Collectors.toList());
}

private void ordenarPostsPorFechaDescendente(List<Post> posts) {
    List<Post> sorted = posts.stream()
        .sorted(Comparator.comparing(Post::getFecha).reversed())
        .collect(Collectors.toList());
    
    posts.clear();
    posts.addAll(sorted);
}

public List<Post> ultimosPosts(Usuario user, int cantidad) {
    List<Post> postsOtrosUsuarios = obtenerPostsOtrosUsuarios(user);
    ordenarPostsPorFechaDescendente(postsOtrosUsuarios);
    return postsOtrosUsuarios.stream().limit(cantidad).collect(Collectors.toList());
}


public class Post {
    // … campos y demás …

    public boolean esDelUsuario(Usuario otro) {
        return this.usuario.equals(otro);
    }
}
```