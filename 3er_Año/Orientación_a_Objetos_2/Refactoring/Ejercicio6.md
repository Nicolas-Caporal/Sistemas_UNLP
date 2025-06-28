# Ejercicio 6 - Árboles binarios
Un árbol binario es una estructura de datos en la que cada nodo puede tener como máximo dos hijos: uno izquierdo y uno derecho. Es común utilizar esta estructura para representar jerarquías o realizar operaciones de búsqueda, recorrido y ordenamiento.

## Código Original
```java
package ar.info.unlp.arboles;

public class ArbolBinario {
    private int valor;
    private ArbolBinario hijoIzquierdo;
    private ArbolBinario hijoDerecho;

    public ArbolBinario(int valor) {
        this.valor = valor;
        this.hijoIzquierdo = null;
        this.hijoDerecho = null;
    }

    public int getValor() {
        return valor;
    }

    public void setValor(int valor) {
        this.valor = valor;
    }

    public ArbolBinario getHijoIzquierdo() {
        return hijoIzquierdo;
    }

    public void setHijoIzquierdo(ArbolBinario hijoIzquierdo) {
        this.hijoIzquierdo = hijoIzquierdo;
    }

    public ArbolBinario getHijoDerecho() {
        return hijoDerecho;
    }

    public void setDerecha(ArbolBinario hijoDerecho) {
        this.hijoDerecho = hijoDerecho;
    }

   public String recorrerPreorden() {
        String resultado = valor + " - ";
        if (this.getHijoIzquierdo() != null) {
        	resultado += this.getHijoIzquierdo().recorrerPreorden();
	}
        if (this.getHijoDerecho() != null) {
        	resultado += this.getHijoDerecho().recorrerPreorden();
        }
        return resultado;
    }

   public String recorrerInorden() {
        String resultado = "";
        if (this.getHijoIzquierdo() != null) {
        	resultado += this.getHijoIzquierdo().recorrerInorden();
	}
        resultado += valor + " - ";
        if (this.getHijoDerecho() != null) {
        	resultado += this.getHijoDerecho().recorrerInorden();
        }
        return resultado;
    }

   public String recorrerPostorden() {
        String resultado = "";
        if (this.getHijoIzquierdo() != null) {
        	resultado += this.getHijoIzquierdo().recorrerPostorden();
	}
        if (this.getHijoDerecho() != null) {
        	resultado += this.getHijoDerecho().recorrerPostorden();
        }
        resultado += valor + " - ";
        return resultado;
    }

}
```

## Bad smell 1: Repeated null checks
### Refactoring: Introduce null objects
```java
package ar.info.unlp.arboles;

public class ArbolBinario {
    private int valor;
    private ArbolBinario hijoIzquierdo;
    private ArbolBinario hijoDerecho;

    public ArbolBinario(int valor) {
        this.valor = valor;
        this.hijoIzquierdo = new NullArbol();;
        this.hijoDerecho = new NullArbol();;
    }

    public int getValor() {
        return valor;
    }

    public void setValor(int valor) {
        this.valor = valor;
    }

    public ArbolBinario getHijoIzquierdo() {
        return hijoIzquierdo;
    }

    public void setHijoIzquierdo(ArbolBinario hijoIzquierdo) {
        this.hijoIzquierdo = hijoIzquierdo;
    }

    public ArbolBinario getHijoDerecho() {
        return hijoDerecho;
    }

    public void setDerecha(ArbolBinario hijoDerecho) {
        this.hijoDerecho = hijoDerecho;
    }

   public String recorrerPreorden() {
        String resultado = valor + " - ";
       	resultado += this.getHijoIzquierdo().recorrerPreorden();
       	resultado += this.getHijoDerecho().recorrerPreorden();
        return resultado;
    }

   public String recorrerInorden() {
        String resultado = "";
      	resultado += this.getHijoIzquierdo().recorrerInorden();
        resultado += valor + " - ";
       	resultado += this.getHijoDerecho().recorrerInorden();
        return resultado;
    }

   public String recorrerPostorden() {
        String resultado = "";
        resultado += this.getHijoIzquierdo().recorrerPostorden();
       	resultado += this.getHijoDerecho().recorrerPostorden();
        resultado += valor + " - ";
        return resultado;
    }

}

package ar.info.unlp.arboles;

public class NullArbol extends ArbolBinario {
    private NullArbol() {
        super(0);  
    }

    @Override
    public String recorrerPreorden() { return ""; }
    @Override
    public String recorrerInorden()  { return ""; }
    @Override
    public String recorrerPostorden(){ return ""; }

    // Para que nunca haya nulls en hijos, devolvemos siempre el mismo NullArbol
    @Override
    public ArbolBinario getHijoIzquierdo() { return this; }
    @Override
    public ArbolBinario getHijoDerecho()   { return this; }
}
```



## Bad smell 2: Temporary variable, en los 3 metodos para recorrer
### Refactoring: Replace temp with query
```java
package ar.info.unlp.arboles;

public class ArbolBinario {
    private int valor;
    private ArbolBinario hijoIzquierdo;
    private ArbolBinario hijoDerecho;

    public ArbolBinario(int valor) {
        this.valor = valor;
        this.hijoIzquierdo = new NullArbol();;
        this.hijoDerecho = new NullArbol();;
    }

    public int getValor() {
        return valor;
    }

    public void setValor(int valor) {
        this.valor = valor;
    }

    public ArbolBinario getHijoIzquierdo() {
        return hijoIzquierdo;
    }

    public void setHijoIzquierdo(ArbolBinario hijoIzquierdo) {
        this.hijoIzquierdo = hijoIzquierdo;
    }

    public ArbolBinario getHijoDerecho() {
        return hijoDerecho;
    }

    public void setDerecha(ArbolBinario hijoDerecho) {
        this.hijoDerecho = hijoDerecho;
    }

   public String recorrerPreorden() {
    return valor + " - " +
           this.getHijoIzquierdo().recorrerPreorden() +
           this.getHijoDerecho().recorrerPreorden();
    }


   public String recorrerInorden() {
    return this.getHijoIzquierdo().recorrerInorden() +
        valor + " - " +
        this.getHijoDerecho().recorrerInorden();
    }


   public String recorrerPostorden() {
    return this.getHijoIzquierdo().recorrerPostorden() +
           this.getHijoDerecho().recorrerPostorden() +
           valor + " - ";
    }
}

package ar.info.unlp.arboles;

public class NullArbol extends ArbolBinario {
    private NullArbol() {
        super(0);  
    }

    @Override
    public String recorrerPreorden() { return ""; }
    @Override
    public String recorrerInorden()  { return ""; }
    @Override
    public String recorrerPostorden(){ return ""; }

    // Para que nunca haya nulls en hijos, devolvemos siempre el mismo NullArbol
    @Override
    public ArbolBinario getHijoIzquierdo() { return this; }
    @Override
    public ArbolBinario getHijoDerecho()   { return this; }
}
```