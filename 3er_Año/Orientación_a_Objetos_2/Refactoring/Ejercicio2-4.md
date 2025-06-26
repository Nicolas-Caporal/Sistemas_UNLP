# Ejercicio 2.3 - Carrito de compras


## Código Original
```java

public class Producto {
    private String nombre;
    private double precio;
    
    public double getPrecio() {
        return this.precio;
    }
}

public class ItemCarrito {
    private Producto producto;
    private int cantidad;
        
    public Producto getProducto() {
        return this.producto;
    }
    
    public int getCantidad() {
        return this.cantidad;
    }

}

public class Carrito {
    private List<ItemCarrito> items;
    
    public double total() {
        return this.items.stream()
        .mapToDouble(item -> 
        item.getProducto().getPrecio() * item.getCantidad())
        .sum();
    }
}
```

## Bad Smell 1: Feature envy y Data class Producto

### Refactoring: Move method
Mecánica:
* Revisar las variables de instancia usadas por el método a mover. ¿Tiene sentido moverlas también?

* Revisar super y subclases por otras declaraciones del método. Si hay otras, tal vez no se pueda mover.

* Crear un nuevo método en la clase target cuyo nombre explique su propósito.

* Copiar el código a mover al nuevo método. Ajustar lo que haga falta.

* Compilar la clase target.

* Determinar cómo referenciar al target desde el source.

* Reemplazar el método original por llamada a método en target.

* Compilar y testear.

* Decidir si remover el método original o mantenerlo como delegación.

```java
public class Producto {
    private String nombre;
    private double precio;
    
    public double getPrecio() {
        return this.precio;
    }

    public double precioPor(int cantidad){
        return this.precio * cantidad;
    }
}

public class ItemCarrito {
    private Producto producto;
    private int cantidad;
        
    public Producto getProducto() {
        return this.producto;
    }
    
    public int getCantidad() {
        return this.cantidad;
    }

}

public class Carrito {
    private List<ItemCarrito> items;
    
    public double total() {
        return this.items.stream()
        .mapToDouble(item -> 
        item.getProducto().precioPor(item.getCantidad()))
        .sum();
    }
}
```

## Bad Smell 2: Feature envy y Data class ItemCarrito

### Refactoring: Move method
Mecánica:
* Revisar las variables de instancia usadas por el método a mover. ¿Tiene sentido moverlas también?

* Revisar super y subclases por otras declaraciones del método. Si hay otras, tal vez no se pueda mover.

* Crear un nuevo método en la clase target cuyo nombre explique su propósito.

* Copiar el código a mover al nuevo método. Ajustar lo que haga falta.

* Compilar la clase target.

* Determinar cómo referenciar al target desde el source.

* Reemplazar el método original por llamada a método en target.

* Compilar y testear.

* Decidir si remover el método original o mantenerlo como delegación.

```java
public class Producto {
    private String nombre;
    private double precio;
    
    public double getPrecio() {
        return this.precio;
    }

    public double precioPor(int cantidad){
        return this.precio * cantidad;
    }
}

public class ItemCarrito {
    private Producto producto;
    private int cantidad;
        
    public Producto getProducto() {
        return this.producto;
    }
    
    public int getCantidad() {
        return this.cantidad;
    }

    public double subtotal() {
        return producto.precioPor(cantidad);
    }
}

public class Carrito {
    private List<ItemCarrito> items;
    
    public double total() {
        return items.stream()
            .mapToDouble(ItemCarrito::subtotal)
            .sum();
    }
}
```