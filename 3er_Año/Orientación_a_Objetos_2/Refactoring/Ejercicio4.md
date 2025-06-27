# Ejercicio 4 - Pedidos
Se tiene el siguiente modelo de un sistema de pedidos y la correspondiente implementación.
```java
01: public class Pedido {
02:  private Cliente cliente;
03:  private List<Producto> productos;
04:  private String formaPago;
05:  public Pedido(Cliente cliente, List<Producto> productos, String formaPago) {
06:     if (!"efectivo".equals(formaPago)
07:        && !"6 cuotas".equals(formaPago)
08:        && !"12 cuotas".equals(formaPago)) {
09:          throw new Error("Forma de pago incorrecta");
10:    }
11:    this.cliente = cliente;
12:    this.productos = productos;
13:    this.formaPago = formaPago;
14:   }
15:   public double getCostoTotal() {
16:     double costoProductos = 0;
17:     for (Producto producto : this.productos) {
18:       costoProductos += producto.getPrecio();
19:     }
20:     double extraFormaPago = 0;
21:     if ("efectivo".equals(this.formaPago)) {
22:       extraFormaPago = 0;
23:     } else if ("6 cuotas".equals(this.formaPago)) {
24:       extraFormaPago = costoProductos * 0.2;
25:     } else if ("12 cuotas".equals(this.formaPago)) {
26:       extraFormaPago = costoProductos * 0.5;
27:     }
28:     int añosDesdeFechaAlta = Period.between(this.cliente.getFechaAlta(), LocalDate.now()).getYears();
29:     // Aplicar descuento del 10% si el cliente tiene más de 5 años de antiguedad
30:     if (añosDesdeFechaAlta > 5) {
31:       return (costoProductos + extraFormaPago) * 0.9;
32:     }
33:     return costoProductos + extraFormaPago;
34:   }
}
36: public class Cliente {
37:   private LocalDate fechaAlta;
38:   public LocalDate getFechaAlta() {
39:     return this.fechaAlta;
40:   }
41: }
42: public class Producto {
43:   private double precio;
44:   public double getPrecio() {
45:     return this.precio;
46:   }
47: }
```


### Tareas:
* Dado el código anterior, aplique únicamente los siguientes refactoring:
1) Replace Loop with Pipeline (líneas 16 a 19)
2) Replace Conditional with Polymorphism (líneas 21 a 27)
3) Extract method y move method (línea 28)
4) Extract method y replace temp with query (líneas 28 a 33)

#### Además:
Realice el diagrama de clases del código refactorizado.

### Replace Loop with Pipeline (líneas 16 a 19)
Mecanica:
* Crear una nueva variable para la colección usada en el bucle.
* Copiar esa colección desde una variable existente, si hace falta.
* Empezar desde arriba e ir reemplazando cada parte del comportamiento del bucle con una operación del pipeline.
* Probar después de cada cambio.
* Una vez que todo el comportamiento esté en el pipeline, eliminar el bucle.
* Si el bucle usaba un acumulador, asignar el resultado del pipeline a esa variable.
```java
01: public class Pedido {
02:  private Cliente cliente;
03:  private List<Producto> productos;
04:  private String formaPago;
05:  public Pedido(Cliente cliente, List<Producto> productos, String formaPago) {
06:     if (!"efectivo".equals(formaPago)
07:        && !"6 cuotas".equals(formaPago)
08:        && !"12 cuotas".equals(formaPago)) {
09:          throw new Error("Forma de pago incorrecta");
10:    }
11:    this.cliente = cliente;
12:    this.productos = productos;
13:    this.formaPago = formaPago;
14:   }
15:   public double getCostoTotal() {
16:     
17:     double costoProductos = this.productos.stream().mapToDouble(producto -> producto.getPrecio()).sum();
18:  
19:     
20:     double extraFormaPago = 0;
21:     if ("efectivo".equals(this.formaPago)) {
22:       extraFormaPago = 0;
23:     } else if ("6 cuotas".equals(this.formaPago)) {
24:       extraFormaPago = costoProductos * 0.2;
25:     } else if ("12 cuotas".equals(this.formaPago)) {
26:       extraFormaPago = costoProductos * 0.5;
27:     }
28:     int añosDesdeFechaAlta = Period.between(this.cliente.getFechaAlta(), LocalDate.now()).getYears();
29:     // Aplicar descuento del 10% si el cliente tiene más de 5 años de antiguedad
30:     if (añosDesdeFechaAlta > 5) {
31:       return (costoProductos + extraFormaPago) * 0.9;
32:     }
33:     return costoProductos + extraFormaPago;
34:   }
}
36: public class Cliente {
37:   private LocalDate fechaAlta;
38:   public LocalDate getFechaAlta() {
39:     return this.fechaAlta;
40:   }
41: }
42: public class Producto {
43:   private double precio;
44:   public double getPrecio() {
45:     return this.precio;
46:   }
47: }
```


### Replace Conditional with Polymorphism (líneas 21 a 27)
Mecánica:
* Crear la jerarquía de clases necesaria
* Si el condicional es parte de un método largo: Extract Method
##### Por cada subclase:
* Crear un método que sobrescribe al método que contiene el condicional
* Copiar el código de la condición correspondiente en el método de la subclase y ajustar
* Compilar y testear
* Borrar la condición y código del branch del método en la superclase
* Compilar y testear
* Hacer que el método en la superclase sea abstracto
  
```java
  public abstract class FormaDePago(){
    public abstract double calcularRecargoPorFormaDePago(double costoProductos);
  }

  public class Efectivo extends FormaDePago(){
    @Override
    public double calcularRecargoPorFormaDePago(double costoProductos){
        return extraFormaPago = 0;
    }
  }

  public class SeisCuotas extends FormaDePago(){
    public double calcularRecargoPorFormaDePago(double costoProductos){
        return extraFormaPago = costoProductos * 0.2;
    }
  }

  public class DoceCuotas extends FormaDePago(){
    public double calcularRecargoPorFormaDePago(double costoProductos){
        return extraFormaPago = costoProductos * 0.5;
    }
  }


01: public class Pedido {
02:  private Cliente cliente;
03:  private List<Producto> productos;
04:  private FormaDePago formaPago;
05:  public Pedido(Cliente cliente, List<Producto> productos, FormaDePago formaPago) {
06:     this.formaPago = formaPago;
11:    this.cliente = cliente;
12:    this.productos = productos;
13:    this.formaPago = formaPago;
14:   }
15:   public double getCostoTotal() {
16:     
17:     double costoProductos = this.productos.stream().mapToDouble(producto -> producto.getPrecio()).sum();
18:  
19:     
20:     double extraFormaPago = this.formaPago.calcularRecargoPorFormaDePago(costoProductos);
21:     
22:     
23:     
24:     
25:     
26:     
27:     
28:     int añosDesdeFechaAlta = Period.between(this.cliente.getFechaAlta(), LocalDate.now()).getYears();
29:     // Aplicar descuento del 10% si el cliente tiene más de 5 años de antiguedad
30:     if (añosDesdeFechaAlta > 5) {
31:       return (costoProductos + extraFormaPago) * 0.9;
32:     }
33:     return costoProductos + extraFormaPago;
34:   }
}
36: public class Cliente {
37:   private LocalDate fechaAlta;
38:   public LocalDate getFechaAlta() {
39:     return this.fechaAlta;
40:   }
41: }
42: public class Producto {
43:   private double precio;
44:   public double getPrecio() {
45:     return this.precio;
46:   }
47: }
```

### Extract method y move method (línea 28)
```java
  public abstract class FormaDePago(){
    public abstract double calcularRecargoPorFormaDePago();
  }

  public class Efectivo extends FormaDePago(){
    @Override
    public double calcularRecargoPorFormaDePago(){
        return extraFormaPago = 0;
    }
  }

  public class SeisCuotas extends FormaDePago(){
    public double calcularRecargoPorFormaDePago(){
        return extraFormaPago = costoProductos * 0.2;
    }
  }

  public class DoceCuotas extends FormaDePago(){
    public double calcularRecargoPorFormaDePago(){
        return extraFormaPago = costoProductos * 0.5;
    }
  }


  public class Cliente{  
    public int getAntiguedad(){   
      return Period.between(this.fechaDeAlta(), LocalDate.now()).getYears();
    }
  }

01: public class Pedido {
02:  private Cliente cliente;
03:  private List<Producto> productos;
04:  private FormaDePago formaPago;
05:  public Pedido(Cliente cliente, List<Producto> productos, FormaDePago formaPago) {
06:     this.formaPago = formaPago;
11:    this.cliente = cliente;
12:    this.productos = productos;
13:    this.formaPago = formaPago;
14:   }
15:   public double getCostoTotal() {
16:     
17:     double costoProductos = this.productos.stream().mapToDouble(producto -> producto.getPrecio()).sum();
18:  
19:     
20:     double extraFormaPago = this.formaPago.calcularRecargoPorFormaDePago();
21:     
22:     
23:     
24:     
25:     
26:     
27:     
28:     int añosDesdeFechaAlta = this.cliente.getAntiguedad();

29:     // Aplicar descuento del 10% si el cliente tiene más de 5 años de antiguedad
30:     if (añosDesdeFechaAlta > 5) {
31:       return (costoProductos + extraFormaPago) * 0.9;
32:     }
33:     return costoProductos + extraFormaPago;
34:   }
}
36: public class Cliente {
37:   private LocalDate fechaAlta;
38:   public LocalDate getFechaAlta() {
39:     return this.fechaAlta;
40:   }
41: }
42: public class Producto {
43:   private double precio;
44:   public double getPrecio() {
45:     return this.precio;
46:   }
47: }
```


### Extract method y replace temp with query (línea 28 a 33)
```java
  public abstract class FormaDePago(){
    public abstract double calcularRecargoPorFormaDePago();
  }

  public class Efectivo extends FormaDePago(){
    @Override
    public double calcularRecargoPorFormaDePago(){
        return extraFormaPago = 0;
    }
  }

  public class SeisCuotas extends FormaDePago(){
    public double calcularRecargoPorFormaDePago(){
        return extraFormaPago = costoProductos * 0.2;
    }
  }

  public class DoceCuotas extends FormaDePago(){
    public double calcularRecargoPorFormaDePago(){
        return extraFormaPago = costoProductos * 0.5;
    }
  }


  public class Cliente{  
    public int getAntiguedad(){   
      return Period.between(this.fechaDeAlta(), LocalDate.now()).getYears();
    }
  }

01: public class Pedido {
02:  private Cliente cliente;
03:  private List<Producto> productos;
04:  private FormaDePago formaPago;

    

05:  public Pedido(Cliente cliente, List<Producto> productos, FormaDePago formaPago) {
06:     this.formaPago = formaPago;
11:    this.cliente = cliente;
12:    this.productos = productos;
13:    this.formaPago = formaPago;
14:   }

      public double aplicarDescuentoSiTienAntiguedad10Años(double costoProductos, double extraFormaPago){
        int añosDesdeFechaAlta = this.cliente.getAntiguedad();
       // Aplicar descuento del 10% si el cliente tiene más de 5 años de antiguedad
       if (añosDesdeFechaAlta > 5) {
         return (costoProductos + extraFormaPago) * 0.9;
       }
       return costoProductos + extraFormaPago;
      }

15:   public double getCostoTotal() {
17:     double costoProductos = this.productos.stream().mapToDouble(producto -> producto.getPrecio()).sum();
20:     double extraFormaPago = this.formaPago.calcularRecargoPorFormaDePago();
27:     
        return this.aplicarDescuentoSiTienAntiguedad10Años(costoProductos, extraFormaPago);
34:   }
}
36: public class Cliente {
37:   private LocalDate fechaAlta;
38:   public LocalDate getFechaAlta() {
39:     return this.fechaAlta;
40:   }
41: }
42: public class Producto {
43:   private double precio;
44:   public double getPrecio() {
45:     return this.precio;
46:   }
47: }
```