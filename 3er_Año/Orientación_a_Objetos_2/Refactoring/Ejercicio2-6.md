# Ejercicio 2.6 - Películas


## Código Original
```java
public class Usuario {
    String tipoSubscripcion;
    // ...

    public void setTipoSubscripcion(String unTipo) {
   	 this.tipoSubscripcion = unTipo;
    }
    
    public double calcularCostoPelicula(Pelicula pelicula) {
   	 double costo = 0;
   	 if (tipoSubscripcion=="Basico") {
   		 costo = pelicula.getCosto() + pelicula.calcularCargoExtraPorEstreno();
   	 }
   	 else if (tipoSubscripcion== "Familia") {
   		 costo = (pelicula.getCosto() + pelicula.calcularCargoExtraPorEstreno()) * 0.90;
   	 }
   	 else if (tipoSubscripcion=="Plus") {
   		 costo = pelicula.getCosto();
   	 }
   	 else if (tipoSubscripcion=="Premium") {
   		 costo = pelicula.getCosto() * 0.75;
   	 }
   	 return costo;
    }
}

public class Pelicula {
    LocalDate fechaEstreno;
    // ...

    public double getCosto() {
   	 return this.costo;
    }
    
    public double calcularCargoExtraPorEstreno(){
      // Si la Película se estrenó 30 días antes de la fecha actual, retorna un cargo de 0$, caso contrario, retorna un cargo extra de 300$
      return (ChronoUnit.DAYS.between(this.fechaEstreno, LocalDate.now()) ) > 30 ? 0 : 300;
    }
}
```

## Bad smell 1: Feature envy
### Refactoring: Move method
```java
public class Usuario {
    String tipoSubscripcion;
    // ...

    public void setTipoSubscripcion(String unTipo) {
   	 this.tipoSubscripcion = unTipo;
    }

    public double calcularCostoPelicula(Pelicula pelicula) {
      return pelicula.calcularCosto();
    }

}

public class Pelicula {
    LocalDate fechaEstreno;
    // ...

    public double getCosto() {
   	 return this.costo;
    }
    
    public double calcularCargoExtraPorEstreno(){
      // Si la Película se estrenó 30 días antes de la fecha actual, retorna un cargo de 0$, caso contrario, retorna un cargo extra de 300$
      return (ChronoUnit.DAYS.between(this.fechaEstreno, LocalDate.now()) ) > 30 ? 0 : 300;
    }

    public double calcularCosto() {
      double costo = 0;
      if (tipoSubscripcion=="Basico") {
        costo = this.costo + pelicula.calcularCargoExtraPorEstreno();
      }
      else if (tipoSubscripcion== "Familia") {
        costo = (this.costo + pelicula.calcularCargoExtraPorEstreno()) * 0.90;
      }
      else if (tipoSubscripcion=="Plus") {
        costo = this.costo;
      }
      else if (tipoSubscripcion=="Premium") {
        costo = this.costo * 0.75;
      }
      return costo;
    }
}
```

## Bad smell 2: Type Code
### Refactoring: Replace conditional logic with strategy
Mecanica:
* Crear una clase Strategy
* Aplicar "Move Method" para mover el cálculo con los condicionales del contexto al Strategy.
Definir una v.i. en el contexto para referenciar al strategy y un setter (generalmente el constructor del contexto).
Dejar un método en el contexto que delegue
Elegir los parámetros necesarios para pasar al strategy (¿se pasa el contexto entero a si mismo? ¿solo algunas variables? ¿en qué momento?)
Compilar y testear

* Aplicar "Extract Parameter" en el código del contexto que inicializa un strategy concreto, para permitir a los clientes setear el strategy.
Compilar y testear.

*  Aplicar "Replace Conditional with Polymorphism" en el método del Strategy.

* Compilar y testear con distintas combinaciones de estrategis y contextos.
  
```java
public class Usuario {
    String tipoSubscripcion;
    // ...

    public void setTipoSubscripcion(String unTipo) {
   	 this.tipoSubscripcion = unTipo;
    }

    public double calcularCostoPelicula(Pelicula pelicula) {
      return pelicula.calcularCosto();
    }

}

public class Pelicula {
    LocalDate fechaEstreno;
    private Suscripcion sub;
    
    public Pelicula (LocalDate fechaEstreno, Suscripcion sub){
      this.fechaEstreno = fechaEstreno;
      this.sub = sub;
    }

    public double getCosto() {
   	 return this.costo;
    }

    public void setSuscripcion(Suscripcion sub) {
   	  this.sub = sub;
    }
    
    public double calcularCargoExtraPorEstreno(){
      // Si la Película se estrenó 30 días antes de la fecha actual, retorna un cargo de 0$, caso contrario, retorna un cargo extra de 300$
      return (ChronoUnit.DAYS.between(this.fechaEstreno, LocalDate.now()) ) > 30 ? 0 : 300;
    }

    public double calcularCosto() {
      return this.sub.calcularCosto(this.costo, this.calcularCargoExtraPorEstreno());
    }
}


public class Suscripcion {
      public double calcularCosto(double costoBase, double extra) {
      double costo = 0;
      if (tipoSubscripcion=="Basico") {
        costo = costoBase + extra;
      }
      else if (tipoSubscripcion== "Familia") {
        costo = (costoBase + extra) * 0.90;
      }
      else if (tipoSubscripcion=="Plus") {
        costo = costoBase;
      }
      else if (tipoSubscripcion=="Premium") {
        costo = costoBase * 0.75;
      }
      return costo;
    }
}
```
> acá ya cree la clase strategy, move method, y extract parameter, quería poner separado lo del Replace Conditional with Polymorfism porque no lo tengo muy claro como se hace, pero forma parte de lo anterior

#### Refactoring: Replace conditional logic with polymorfism
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
public class Usuario {
    String tipoSubscripcion;
    // ...

    public void setTipoSubscripcion(String unTipo) {
   	 this.tipoSubscripcion = unTipo;
    }

    public double calcularCostoPelicula(Pelicula pelicula) {
      return pelicula.calcularCosto();
    }

}

public class Pelicula {
    LocalDate fechaEstreno;
    private Suscripcion sub;
    
    public Pelicula (LocalDate fechaEstreno, Suscripcion sub){
      this.fechaEstreno = fechaEstreno;
      this.sub = sub;
    }

    public double getCosto() {
   	 return this.costo;
    }

    public void setSuscripcion(Suscripcion sub) {
   	  this.sub = sub;
    }
    
    public double calcularCargoExtraPorEstreno(){
      // Si la Película se estrenó 30 días antes de la fecha actual, retorna un cargo de 0$, caso contrario, retorna un cargo extra de 300$
      return (ChronoUnit.DAYS.between(this.fechaEstreno, LocalDate.now()) ) > 30 ? 0 : 300;
    }

    public double calcularCosto() {
      return this.sub.calcularCosto(this.costo, this.calcularCargoExtraPorEstreno());
    }
}


public abstract class Suscripcion {
      public abstract double calcularCosto(double costoBase, double extra);
}

public class SuscripcionBasico extends Suscripcion {
  @Override
  public double calcularCosto(double costoBase, double extra) {
    double costo = 0;
    costo = costoBase + extra;
    return costo;
  }
}

public class SuscripcionFamilia extends Suscripcion {
  @Override
  public double calcularCosto(double costoBase, double extra) {
    double costo = 0;
    costo = (costoBase + extra) * 0.90;
    return costo;
  }
}

public class SuscripcionPlus extends Suscripcion {
  @Override
  public double calcularCosto(double costoBase, double extra) {
    double costo = 0;
    costo = costoBase;
    return costo;
  }
}

public class SuscripcionPremium extends Suscripcion {
  @Override
  public double calcularCosto(double costoBase, double extra) {
    double costo = 0;
    costo = costoBase * 0.75;
    return costo;
  }
}
```


## Bad smell 3: Temporary Variable
Variable costo en las 3 subclases de la estrategia
### Refactoring: Inline variable
Mecanica:
* Verificar que el lado derecho de la asignación no tenga efectos secundarios
* Si la variable no es inmutable, declararla como tal y probar
* Reemplazar la primera referencia a la variable con la expresión del lado derecho de la asignación
* Testear
* Repetir el reemplazo para todas las referencias restantes a la variable
* Eliminar la declaración y asignación de la variable
* Testear

```java
public class Usuario {
    String tipoSubscripcion;
    // ...

    public void setTipoSubscripcion(String unTipo) {
   	 this.tipoSubscripcion = unTipo;
    }

    public double calcularCostoPelicula(Pelicula pelicula) {
      return pelicula.calcularCosto();
    }

}

public class Pelicula {
    LocalDate fechaEstreno;
    private Suscripcion sub;
    
    public Pelicula (LocalDate fechaEstreno, Suscripcion sub){
      this.fechaEstreno = fechaEstreno;
      this.sub = sub;
    }

    public double getCosto() {
   	 return this.costo;
    }

    public void setSuscripcion(Suscripcion sub) {
   	  this.sub = sub;
    }
    
    public double calcularCargoExtraPorEstreno(){
      // Si la Película se estrenó 30 días antes de la fecha actual, retorna un cargo de 0$, caso contrario, retorna un cargo extra de 300$
      return (ChronoUnit.DAYS.between(this.fechaEstreno, LocalDate.now()) ) > 30 ? 0 : 300;
    }

    public double calcularCosto() {
      return this.sub.calcularCosto(this.costo, this.calcularCargoExtraPorEstreno());
    }
}


public abstract class Suscripcion {
      public abstract double calcularCosto(double costoBase, double extra);
}

public class SuscripcionBasico extends Suscripcion {
  @Override
  public double calcularCosto(double costoBase, double extra) {
    return costoBase + extra;
  }
}

public class SuscripcionFamilia extends Suscripcion {
  @Override
  public double calcularCosto(double costoBase, double extra) {
    return (costoBase + extra) * 0.90;;
  }
}

public class SuscripcionPlus extends Suscripcion {
  @Override
  public double calcularCosto(double costoBase, double extra) {
    return costoBase;
  }
}

public class SuscripcionPremium extends Suscripcion {
  @Override
  public double calcularCosto(double costoBase, double extra) {
    return costoBase * 0.75;;
  }
}
```