# Ejercicio 2.5 - Envio de pedidos


## Código Original
```java
public class Supermercado {
   public void notificarPedido(long nroPedido, Cliente cliente) {
     String notificacion = MessageFormat.format(“Estimado cliente, se le informa que hemos recibido su pedido con número {0}, el cual será enviado a la dirección {1}”, new Object[] { nroPedido, cliente.getDireccionFormateada() });

     // lo imprimimos en pantalla, podría ser un mail, SMS, etc..
    System.out.println(notificacion);
  }
}

public class Cliente {
   public String getDireccionFormateada() {
	return 
		this.direccion.getLocalidad() + “, ” +
		this.direccion.getCalle() + “, ” +
		this.direccion.getNumero() + “, ” +
		this.direccion.getDepartamento()
      ;
}
```

## Bad smell 1: Inappropriate Encapsulation
### Refactoring: Encapsulate variable
Mecanica:
* Crear funciones de acceso (getter) y actualización (setter) para la variable.
* Ejecutar verificaciones estáticas (análisis de código).
* Reemplazar cada referencia directa a la variable con llamadas a las nuevas funciones.
* Probar después de cada reemplazo.
* Restringir la visibilidad de la variable (ej., de public a private).
* Si no se puede eliminar el acceso directo, renombrar la variable y ejecutar pruebas para detectar usos no migrados.
* Probar.
* Si la variable es un registro, considerar aplicar Encapsular Registro (162).

```java
public class Supermercado {
   public void notificarPedido(long nroPedido, Cliente cliente) {
     String notificacion = MessageFormat.format(“Estimado cliente, se le informa que hemos recibido su pedido con número {0}, el cual será enviado a la dirección {1}”, new Object[] { nroPedido, cliente.getDireccionFormateada() });

     // lo imprimimos en pantalla, podría ser un mail, SMS, etc..
    System.out.println(notificacion);
  }
}

public class Cliente {
   public String getDireccionFormateada() {
	return 
		this.direccion.getLocalidad() + “, ” +
		this.direccion.getCalle() + “, ” +
		this.direccion.getNumero() + “, ” +
		this.direccion.getDepartamento();
}

public class Direccion{
    private String localidad;
    private String calle;
    private String numero;
    private String departamento;

    public String getLocalidad(){
        retur this.localidad;
        // ...
    }
}
```


## Bad Smell 2: Feature envy Cliente y Data class Direccion

### Refactoring: Move fields
Mecánica:
* Asegurar que el campo origen esté encapsulado (usar getters/setters).
* Probar.
* Crear el campo (y sus métodos de acceso) en el objeto destino.
* Ejecutar verificaciones estáticas.
* Asegurar una referencia del objeto origen al destino:
* Usar un campo/método existente que proporcione el destino.
* Si no existe, crear un método temporal o un campo en el origen para almacenar el destino.
* Ajustar los métodos de acceso para usar el campo destino:
* Si el destino es compartido entre múltiples objetos origen, actualizar el setter para modificar ambos campos y añadir Introducir Aserción (302) para detectar inconsistencias.
* Probar.
* Eliminar el campo origen.
* Probar.

```java
public class Supermercado {
   public void notificarPedido(long nroPedido, Cliente cliente) {
     String notificacion = MessageFormat.format(“Estimado cliente, se le informa que hemos recibido su pedido con número {0}, el cual será enviado a la dirección {1}”, new Object[] { nroPedido, cliente.getDireccionFormateada() });

     // lo imprimimos en pantalla, podría ser un mail, SMS, etc..
    System.out.println(notificacion);
  }
}

public class Cliente {
    public String localidad;
    public String calle;
    public String numero;
    public String departamento;

   public String getDireccionFormateada() {
	return 
		this.localidad() + “, ” +
		this.calle() + “, ” +
		this.numero() + “, ” +
		this.departamento();
}
```