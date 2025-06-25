# Ejercicio 1.3 - Cálculos 
Analice el código que se muestra a continuación. Indique qué code smells encuentra y cómo pueden corregirse.	
```java
public void imprimirValores() {
	int totalEdades = 0;
	double promedioEdades = 0;
	double totalSalarios = 0;
	
	for (Empleado empleado : personal) {
		totalEdades = totalEdades + empleado.getEdad();
		totalSalarios = totalSalarios + empleado.getSalario();
	}
	promedioEdades = totalEdades / personal.size();
		
	String message = String.format("El promedio de las edades es %s y el total de salarios es %s", promedioEdades, totalSalarios);
	
	System.out.println(message);
}
```

## 1. Bad smell: Bad name
## Refactoring: RenameMethod
```java
@Override
public void toString() {
	int totalEdades = 0;
	double promedioEdades = 0;
	double totalSalarios = 0;
	
	for (Empleado empleado : personal) {
		totalEdades = totalEdades + empleado.getEdad();
		totalSalarios = totalSalarios + empleado.getSalario();
	}
	promedioEdades = totalEdades / personal.size();
		
	String message = String.format("El promedio de las edades es %s y el total de salarios es %s", promedioEdades, totalSalarios);
	
	System.out.println(message);
}
```

## 2. Bad smell: Temporary Variables en promedioEdades
## Refactoring: Replace temp With Query
Mecanica:
* Verificar que la variable esté completamente determinada antes de ser usada, y que el código que la calcula no dé un valor distinto cada vez que se ejecuta.
* Si la variable no es de solo lectura (readonly), y puede hacerse readonly, hacerlo.
* Probar que todo siga funcionando.
* Extraer la asignación de esa variable en una función.
* Si la variable y la función no pueden tener el mismo nombre, poner un nombre temporal a la función.
* Asegurarse de que la función extraída no tenga efectos secundarios. Si los tiene, usar el patrón Separate Query from Modifier.
* Testear.
* Usar el patrón Inline Variable para eliminar la variable temporal (o sea, llamar directamente al metodo donde se lo necesita)

```java

private double calcularPromedioEdades() {
	int totalEdades = 0;
	for (Empleado empleado : personal) {
		totalEdades += empleado.getEdad();
	}
	return (double) totalEdades / personal.size();
}

@Override
public void toString() {
	int totalEdades = 0;
	double totalSalarios = 0;
	
	for (Empleado empleado : personal) {
		totalEdades = totalEdades + empleado.getEdad();
		totalSalarios = totalSalarios + empleado.getSalario();
	}
		
	String message = String.format("El promedio de las edades es %s y el total de salarios es %s", calcularPromedioEdades(), totalSalarios);
	
	System.out.println(message);
}
```

## 3. Bad smell: Temporary Variables en totalSalarios
## Refactoring: Replace temp With Query
Mecanica:
* Verificar que la variable esté completamente determinada antes de ser usada, y que el código que la calcula no dé un valor distinto cada vez que se ejecuta.
* Si la variable no es de solo lectura (readonly), y puede hacerse readonly, hacerlo.
* Probar que todo siga funcionando.
* Extraer la asignación de esa variable en una función.
* Si la variable y la función no pueden tener el mismo nombre, poner un nombre temporal a la función.
* Asegurarse de que la función extraída no tenga efectos secundarios. Si los tiene, usar el patrón Separate Query from Modifier.
* Testear.
* Usar el patrón Inline Variable para eliminar la variable temporal (o sea, llamar directamente al metodo donde se lo necesita).

```java

private double calcularPromedioEdades() {
	int totalEdades = 0;
	for (Empleado empleado : personal) {
		totalEdades += empleado.getEdad();
	}
	return (double) totalEdades / personal.size();
}

private double calcularTotalSalarios() {
	double total = 0;
	for (Empleado empleado : personal) {
		total += empleado.getSalario();
	}
	return total;
}


@Override
public void toString() {		
	String message = String.format("El promedio de las edades es %s y el total de salarios es %s", calcularPromedioEdades(), calcularTotalSalarios());
	
	System.out.println(message);
}
```


## 4. Bad smell: Temporary Variables en totalEdades
## Refactoring: Replace temp With Query
Mecanica:
* Verificar que la variable esté completamente determinada antes de ser usada, y que el código que la calcula no dé un valor distinto cada vez que se ejecuta.
* Si la variable no es de solo lectura (readonly), y puede hacerse readonly, hacerlo.
* Probar que todo siga funcionando.
* Extraer la asignación de esa variable en una función.
* Si la variable y la función no pueden tener el mismo nombre, poner un nombre temporal a la función.
* Asegurarse de que la función extraída no tenga efectos secundarios. Si los tiene, usar el patrón Separate Query from Modifier.
* Testear.
* Usar el patrón Inline Variable para eliminar la variable temporal (o sea, llamar directamente al metodo donde se lo necesita).

```java

private double calcularTotalEdades(){
    int totalEdades = 0;
    for (Empleado empleado : personal) {
		totalEdades += empleado.getEdad();
	}
    return totalEdades;
}

private double calcularPromedioEdades() {
	return (double) calcularTotalEdades(); / personal.size();
}

private double calcularTotalSalarios() {
	double total = 0;
	for (Empleado empleado : personal) {
		total += empleado.getSalario();
	}
	return total;
}


@Override
public void toString() {		
	String message = String.format("El promedio de las edades es %s y el total de salarios es %s", calcularPromedioEdades(), calcularTotalSalarios());
	
	System.out.println(message);
}
```

## 5. Bad smell: Reinventar la rueda en calcularTotalSalarios
## Refactoring: Replace Loop with Pipeline
Mecanica:
* Crear una nueva variable para la colección usada en el bucle.
* Copiar esa colección desde una variable existente, si hace falta.
* Empezar desde arriba e ir reemplazando cada parte del comportamiento del bucle con una operación del pipeline.
* Probar después de cada cambio.
* Una vez que todo el comportamiento esté en el pipeline, eliminar el bucle.
* Si el bucle usaba un acumulador, asignar el resultado del pipeline a esa variable.

```java

private double calcularTotalEdades(){
    int totalEdades = 0;
    for (Empleado empleado : personal) {
		totalEdades += empleado.getEdad();
	}
    return totalEdades;
}

private double calcularPromedioEdades() {
	return (double) calcularTotalEdades(); / personal.size();
}

private double calcularTotalSalarios() {
	List<Empleado> empleados = personal;

	double total = empleados.stream()
		.mapToDouble(Empleado::getSalario)
		.sum();

	return total;
}



@Override
public void toString() {		
	String message = String.format("El promedio de las edades es %s y el total de salarios es %s", calcularPromedioEdades(), calcularTotalSalarios());
	
	System.out.println(message);
}
```

## 6. Bad smell: Reinventar la rueda en calcularTotalEdades
## Refactoring: Replace Loop with Pipeline
Mecanica:
* Crear una nueva variable para la colección usada en el bucle.
* Copiar esa colección desde una variable existente, si hace falta.
* Empezar desde arriba e ir reemplazando cada parte del comportamiento del bucle con una operación del pipeline.
* Probar después de cada cambio.
* Una vez que todo el comportamiento esté en el pipeline, eliminar el bucle.
* Si el bucle usaba un acumulador, asignar el resultado del pipeline a esa variable.

```java

private double calcularTotalEdades() {
	List<Empleado> empleados = personal;

	int totalEdades = empleados.stream()
		.mapToInt(Empleado::getEdad)
		.sum();

	return totalEdades;
}


private double calcularPromedioEdades() {
	return (double) calcularTotalEdades(); / personal.size();
}

private double calcularTotalSalarios() {
	List<Empleado> empleados = personal;

	double total = empleados.stream()
		.mapToDouble(Empleado::getSalario)
		.sum();

	return total;
}



@Override
public void toString() {		
	String message = String.format("El promedio de las edades es %s y el total de salarios es %s", calcularPromedioEdades(), calcularTotalSalarios());
	
	System.out.println(message);
}
```