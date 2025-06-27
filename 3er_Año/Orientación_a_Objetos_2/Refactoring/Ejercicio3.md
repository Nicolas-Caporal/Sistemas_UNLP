# Ejercicio 3 - Documentos y estadísticas
Dado el siguiente código implementado en la clase Document y que calcula algunas estadísticas del mismo
Tareas:
1) Enumere los code smell y que refactorings utilizará para solucionarlos.
2) Aplique los refactorings encontrados, mostrando el código refactorizado luego de aplicar cada uno.
3) Analice el código original y detecte si existe un problema al calcular las estadísticas. Explique cuál es el error y en qué casos se da ¿El error identificado sigue presente luego de realizar los refactorings? En caso de que no esté presente, ¿en qué momento se resolvió? De acuerdo a lo visto en la teoría, ¿podemos considerar esto un refactoring?

## Código Original
```java
public class Document {
    List<String> words;
  
    public long characterCount() {
 	long count = this.words
              .stream()
              .mapToLong(w -> w.length())
              .sum();
    	return count;
	}
    public long calculateAvg() {
    	long avgLength = this.words
                      .stream()
                      .mapToLong(w -> w.length())
                      .sum() / this.words.size();
 	return avgLength;
	}
// Resto del código que no importa
}
```


## Bad smell 1: Public variable
La v.i. words es publica
### Refactoring: Encapsulate field
Mecánica:
* Crear funciones de encapsulación (getter/setter) para acceder y modificar la variable
* Ejecutar verificaciones estáticas del código
* Reemplazar cada referencia directa a la variable con llamadas a las funciones de encapsulación correspondientes
* Probar después de cada reemplazo
* Restringir la visibilidad de la variable (ej: cambiar de público a privado)
* Si no se puede evitar completamente el acceso directo:
* Renombrar la variable para detectar referencias restantes
* Probar para identificar usos no migrados
* Realizar pruebas finales
* Si la variable es un registro, considerar aplicar el patrón Encapsulate Record 

```java
public class Document {
    private List<String> words;
  
    public void setWords(List<String> words){
      this.words = words;
    }

    public void getWords(List<String> words){
      return this.words;
    }


    public long characterCount() {
 	    long count = this.words
              .stream()
              .mapToLong(w -> w.length())
              .sum();
    	return count;
	}

    public long calculateAvg() {
    	long avgLength = this.words
                      .stream()
                      .mapToLong(w -> w.length())
                      .sum() / this.words.size();
 	  return avgLength;
	}
// Resto del código que no importa
}
```

## Bad smell 2: Duplicate code
Se repite la manera de calcular la cantidad de caracteres de cada palabra en los 2 metodos
### Refactoring: Extract method
Mecánica:
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
public class Document {
    private List<String> words;
  
    public void setWords(List<String> words){
      this.words = words;
    }

    public void getWords(List<String> words){
      return this.words;
    }


    public long characterCount() {
 	    long count = this.words
              .stream()
              .mapToLong(w -> w.length())
              .sum();
    	return count;
	}

    public long calculateAvg() {
    	long avgLength = this.characterCount() / this.words.size();
 	    return avgLength;
	}
// Resto del código que no importa
}
```


## Bad smell 3: Temporary variable
Los 2 metodos tiene nuna variable temporal
### Refactoring: Inline variable
```java
public class Document {
    private List<String> words;
  
    public void setWords(List<String> words){
      this.words = words;
    }

    public void getWords(List<String> words){
      return this.words;
    }


    public long characterCount() {
    	return this.words
              .stream()
              .mapToLong(w -> w.length())
              .sum();;
	  }

    public long calculateAvg() {
 	    return this.characterCount() / this.words.size();;
	  }
// Resto del código que no importa
}
```