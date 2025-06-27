# Ejercicio 5
### AKA el peor codigo que vas a ver en tu vida

```java

package ar.edu.unlp.info.oo2.facturacion_llamadas;

public class Llamada {
	private String tipoDeLlamada;
	private String origen;
	private String destino;
	private int duracion;

	public Llamada(String tipoLlamada, String origen, String destino, int duracion) {
		this.tipoDeLlamada = tipoLlamada;
		this.origen= origen;
		this.destino= destino;
		this.duracion = duracion;
	}

	public String getTipoDeLlamada() {
		return tipoDeLlamada;
	}

	public String getRemitente() {
		return destino;
	}

	public int getDuracion() {
		return this.duracion;
	}

	public String getOrigen() {
		return origen;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.TreeSet;
import java.util.ArrayList;
import java.util.Random;
import java.util.SortedSet;

public class GestorNumerosDisponibles {
	private SortedSet<String> lineas = new TreeSet<String>();
	private String tipoGenerador = "ultimo";

	public SortedSet<String> getLineas() {
		return lineas;
	}

	public String obtenerNumeroLibre() {
		String linea;
		switch (tipoGenerador) {
			case "ultimo":
				linea = lineas.last();
				lineas.remove(linea);
				return linea;
			case "primero":
				linea = lineas.first();
				lineas.remove(linea);
				return linea;
			case "random":
				linea = new ArrayList<String>(lineas)
						.get(new Random().nextInt(lineas.size()));
				lineas.remove(linea);
				return linea;
		}
		return null;
	}

	public void cambiarTipoGenerador(String valor) {
		this.tipoGenerador = valor;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Empresa {
	private List<Cliente> clientes = new ArrayList<Cliente>();
	private List<Llamada> llamadas = new ArrayList<Llamada>();
	private GestorNumerosDisponibles guia = new GestorNumerosDisponibles();

	static double descuentoJur = 0.15;
	static double descuentoFis = 0;

	public boolean agregarNumeroTelefono(String str) {
		boolean encontre = guia.getLineas().contains(str);
		if (!encontre) {
			guia.getLineas().add(str);
			encontre= true;
			return encontre;
		}
		else {
			encontre= false;
			return encontre;
		}
	}

	public String obtenerNumeroLibre() {
		return guia.obtenerNumeroLibre();
	}

	public Cliente registrarUsuario(String data, String nombre, String tipo) {
		Cliente var = new Cliente();
		if (tipo.equals("fisica")) {
			var.setNombre(nombre);
			String tel = this.obtenerNumeroLibre();
			var.setTipo(tipo);
			var.setNumeroTelefono(tel);
			var.setDNI(data);
		}
		else if (tipo.equals("juridica")) {
			String tel = this.obtenerNumeroLibre();
			var.setNombre(nombre);
			var.setTipo(tipo);
			var.setNumeroTelefono(tel);
			var.setCuit(data);
		}
		clientes.add(var);
		return var;
	}

	public Llamada registrarLlamada(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new Llamada(t, origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public double calcularMontoTotalLlamadas(Cliente cliente) {
		double c = 0;
		for (Llamada l : cliente.llamadas) {
			double auxc = 0;
			if (l.getTipoDeLlamada() == "nacional") {
				// el precio es de 3 pesos por segundo más IVA sin adicional por establecer la llamada
				auxc += l.getDuracion() * 3 + (l.getDuracion() * 3 * 0.21);
			} else if (l.getTipoDeLlamada() == "internacional") {
				// el precio es de 150 pesos por segundo más IVA más 50 pesos por establecer la llamada
				auxc += l.getDuracion() * 150 + (l.getDuracion() * 150 * 0.21) + 50;
			}

			if (cliente.getTipo() == "fisica") {
				auxc -= auxc*descuentoFis;
			} else if(cliente.getTipo() == "juridica") {
				auxc -= auxc*descuentoJur;
			}
			c += auxc;
		}
		return c;
	}

	public int cantidadDeUsuarios() {
		return clientes.size();
	}

	public boolean existeUsuario(Cliente persona) {
		return clientes.contains(persona);
	}

	public GestorNumerosDisponibles getGestorNumeros() {
		return this.guia;
	}
}

package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Cliente {
	public List<Llamada> llamadas = new ArrayList<Llamada>();
	private String tipo;
	private String nombre;
	private String numeroTelefono;
	private String cuit;
	private String dni;

	public String getTipo() {
		return tipo;
	}
	public void setTipo(String tipo) {
		this.tipo = tipo;
	}
	public String getNombre() {
		return nombre;
	}
	public void setNombre(String nombre) {
		this.nombre = nombre;
	}
	public String getNumeroTelefono() {
		return numeroTelefono;
	}
	public void setNumeroTelefono(String numeroTelefono) {
		this.numeroTelefono = numeroTelefono;
	}
	public String getCuit() {
		return cuit;
	}
	public void setCuit(String cuit) {
		this.cuit = cuit;
	}
	public String getDNI() {
		return dni;
	}
	public void setDNI(String dni) {
		this.dni = dni;
	}
}
```


## Bad smell: 
* Long method calcularMontoLlamdas. 

### Refactoring: Extract method del calculo
```java
package ar.edu.unlp.info.oo2.facturacion_llamadas;

public class Llamada {
	private String tipoDeLlamada;
	private String origen;
	private String destino;
	private int duracion;

	public Llamada(String tipoLlamada, String origen, String destino, int duracion) {
		this.tipoDeLlamada = tipoLlamada;
		this.origen= origen;
		this.destino= destino;
		this.duracion = duracion;
	}

	public String getTipoDeLlamada() {
		return tipoDeLlamada;
	}

	public String getRemitente() {
		return destino;
	}

	public int getDuracion() {
		return this.duracion;
	}

	public String getOrigen() {
		return origen;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.TreeSet;
import java.util.ArrayList;
import java.util.Random;
import java.util.SortedSet;

public class GestorNumerosDisponibles {
	private SortedSet<String> lineas = new TreeSet<String>();
	private String tipoGenerador = "ultimo";

	public SortedSet<String> getLineas() {
		return lineas;
	}

	public String obtenerNumeroLibre() {
		String linea;
		switch (tipoGenerador) {
			case "ultimo":
				linea = lineas.last();
				lineas.remove(linea);
				return linea;
			case "primero":
				linea = lineas.first();
				lineas.remove(linea);
				return linea;
			case "random":
				linea = new ArrayList<String>(lineas)
						.get(new Random().nextInt(lineas.size()));
				lineas.remove(linea);
				return linea;
		}
		return null;
	}

	public void cambiarTipoGenerador(String valor) {
		this.tipoGenerador = valor;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Empresa {
	private List<Cliente> clientes = new ArrayList<Cliente>();
	private List<Llamada> llamadas = new ArrayList<Llamada>();
	private GestorNumerosDisponibles guia = new GestorNumerosDisponibles();

	static double descuentoJur = 0.15;
	static double descuentoFis = 0;

	public boolean agregarNumeroTelefono(String str) {
		boolean encontre = guia.getLineas().contains(str);
		if (!encontre) {
			guia.getLineas().add(str);
			encontre= true;
			return encontre;
		}
		else {
			encontre= false;
			return encontre;
		}
	}

	public String obtenerNumeroLibre() {
		return guia.obtenerNumeroLibre();
	}

	public Cliente registrarUsuario(String data, String nombre, String tipo) {
		Cliente var = new Cliente();
		if (tipo.equals("fisica")) {
			var.setNombre(nombre);
			String tel = this.obtenerNumeroLibre();
			var.setTipo(tipo);
			var.setNumeroTelefono(tel);
			var.setDNI(data);
		}
		else if (tipo.equals("juridica")) {
			String tel = this.obtenerNumeroLibre();
			var.setNombre(nombre);
			var.setTipo(tipo);
			var.setNumeroTelefono(tel);
			var.setCuit(data);
		}
		clientes.add(var);
		return var;
	}

	public Llamada registrarLlamada(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new Llamada(t, origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public double calcularMontoLlamada(Llamada llamada, Cliente cliente){
		double auxc = 0;
			if (l.getTipoDeLlamada() == "nacional") {
				// el precio es de 3 pesos por segundo más IVA sin adicional por establecer la llamada
				auxc += l.getDuracion() * 3 + (l.getDuracion() * 3 * 0.21);
			} else if (l.getTipoDeLlamada() == "internacional") {
				// el precio es de 150 pesos por segundo más IVA más 50 pesos por establecer la llamada
				auxc += l.getDuracion() * 150 + (l.getDuracion() * 150 * 0.21) + 50;
			}

			if (cliente.getTipo() == "fisica") {
				auxc -= auxc*descuentoFis;
			} else if(cliente.getTipo() == "juridica") {
				auxc -= auxc*descuentoJur;
			}
			c += auxc;
	}

	public double calcularMontoTotalLlamadas(Cliente cliente) {
		double c = 0;
		for (Llamada l : cliente.llamadas) {
			c += this.calcularMontoLlamada(l, cliente);
		}
		return ;
	}

	public int cantidadDeUsuarios() {
		return clientes.size();
	}

	public boolean existeUsuario(Cliente persona) {
		return clientes.contains(persona);
	}

	public GestorNumerosDisponibles getGestorNumeros() {
		return this.guia;
	}
}

package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Cliente {
	public List<Llamada> llamadas = new ArrayList<Llamada>();
	private String tipo;
	private String nombre;
	private String numeroTelefono;
	private String cuit;
	private String dni;

	public String getTipo() {
		return tipo;
	}
	public void setTipo(String tipo) {
		this.tipo = tipo;
	}
	public String getNombre() {
		return nombre;
	}
	public void setNombre(String nombre) {
		this.nombre = nombre;
	}
	public String getNumeroTelefono() {
		return numeroTelefono;
	}
	public void setNumeroTelefono(String numeroTelefono) {
		this.numeroTelefono = numeroTelefono;
	}
	public String getCuit() {
		return cuit;
	}
	public void setCuit(String cuit) {
		this.cuit = cuit;
	}
	public String getDNI() {
		return dni;
	}
	public void setDNI(String dni) {
		this.dni = dni;
	}
}
```


## Bad smell: 
* Llamada es Data Class.
* Y hay feature envy de Empresa, por método calcularMontoLlamada 

### Refactoring: Move method
```java
package ar.edu.unlp.info.oo2.facturacion_llamadas;

public class Llamada {
	private String tipoDeLlamada;
	private String origen;
	private String destino;
	private int duracion;

	public Llamada(String tipoLlamada, String origen, String destino, int duracion) {
		this.tipoDeLlamada = tipoLlamada;
		this.origen= origen;
		this.destino= destino;
		this.duracion = duracion;
	}

	public String getTipoDeLlamada() {
		return tipoDeLlamada;
	}

	public String getRemitente() {
		return destino;
	}

	public int getDuracion() {
		return this.duracion;
	}

	public String getOrigen() {
		return origen;
	}

	public double calcularMonto(Cliente cliente, double descuentoFis, double descuentoJur){
		double auxc = 0;
			if (this.getTipoDeLlamada() == "nacional") {
				// el precio es de 3 pesos por segundo más IVA sin adicional por establecer la llamada
				auxc += this.getDuracion() * 3 + (this.getDuracion() * 3 * 0.21);
			} else if (this.getTipoDeLlamada() == "internacional") {
				// el precio es de 150 pesos por segundo más IVA más 50 pesos por establecer la llamada
				auxc += this.getDuracion() * 150 + (this.getDuracion() * 150 * 0.21) + 50;
			}

			if (cliente.getTipo() == "fisica") {
				auxc -= auxc*descuentoFis;
			} else if(cliente.getTipo() == "juridica") {
				auxc -= auxc*descuentoJur;
			}
			c += auxc;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.TreeSet;
import java.util.ArrayList;
import java.util.Random;
import java.util.SortedSet;

public class GestorNumerosDisponibles {
	private SortedSet<String> lineas = new TreeSet<String>();
	private String tipoGenerador = "ultimo";

	public SortedSet<String> getLineas() {
		return lineas;
	}

	public String obtenerNumeroLibre() {
		String linea;
		switch (tipoGenerador) {
			case "ultimo":
				linea = lineas.last();
				lineas.remove(linea);
				return linea;
			case "primero":
				linea = lineas.first();
				lineas.remove(linea);
				return linea;
			case "random":
				linea = new ArrayList<String>(lineas)
						.get(new Random().nextInt(lineas.size()));
				lineas.remove(linea);
				return linea;
		}
		return null;
	}

	public void cambiarTipoGenerador(String valor) {
		this.tipoGenerador = valor;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Empresa {
	private List<Cliente> clientes = new ArrayList<Cliente>();
	private List<Llamada> llamadas = new ArrayList<Llamada>();
	private GestorNumerosDisponibles guia = new GestorNumerosDisponibles();

	static double descuentoJur = 0.15;
	static double descuentoFis = 0;

	public boolean agregarNumeroTelefono(String str) {
		boolean encontre = guia.getLineas().contains(str);
		if (!encontre) {
			guia.getLineas().add(str);
			encontre= true;
			return encontre;
		}
		else {
			encontre= false;
			return encontre;
		}
	}

	public String obtenerNumeroLibre() {
		return guia.obtenerNumeroLibre();
	}

	public Cliente registrarUsuario(String data, String nombre, String tipo) {
		Cliente var = new Cliente();
		if (tipo.equals("fisica")) {
			var.setNombre(nombre);
			String tel = this.obtenerNumeroLibre();
			var.setTipo(tipo);
			var.setNumeroTelefono(tel);
			var.setDNI(data);
		}
		else if (tipo.equals("juridica")) {
			String tel = this.obtenerNumeroLibre();
			var.setNombre(nombre);
			var.setTipo(tipo);
			var.setNumeroTelefono(tel);
			var.setCuit(data);
		}
		clientes.add(var);
		return var;
	}

	public Llamada registrarLlamada(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new Llamada(t, origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public double calcularMontoTotalLlamadas(Cliente cliente) {
		double c = 0;
		for (Llamada l : cliente.llamadas) {
			c += l.calcularMonto(cliente, descuentoFis, descuentoJur);
		}
		return ;
	}

	public int cantidadDeUsuarios() {
		return clientes.size();
	}

	public boolean existeUsuario(Cliente persona) {
		return clientes.contains(persona);
	}

	public GestorNumerosDisponibles getGestorNumeros() {
		return this.guia;
	}
}

package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Cliente {
	public List<Llamada> llamadas = new ArrayList<Llamada>();
	private String tipo;
	private String nombre;
	private String numeroTelefono;
	private String cuit;
	private String dni;

	public String getTipo() {
		return tipo;
	}
	public void setTipo(String tipo) {
		this.tipo = tipo;
	}
	public String getNombre() {
		return nombre;
	}
	public void setNombre(String nombre) {
		this.nombre = nombre;
	}
	public String getNumeroTelefono() {
		return numeroTelefono;
	}
	public void setNumeroTelefono(String numeroTelefono) {
		this.numeroTelefono = numeroTelefono;
	}
	public String getCuit() {
		return cuit;
	}
	public void setCuit(String cuit) {
		this.cuit = cuit;
	}
	public String getDNI() {
		return dni;
	}
	public void setDNI(String dni) {
		this.dni = dni;
	}
}
```


## Bad smell: 
* El nuevo metodoCalcularMonto en la clase Llamada es largo. Long method

### Refactoring: Extract method de los dos condicionales, el de tipo de llamada y el de tipo de cliente
```java
package ar.edu.unlp.info.oo2.facturacion_llamadas;

public class Llamada {
	private String tipoDeLlamada;
	private String origen;
	private String destino;
	private int duracion;

	public Llamada(String tipoLlamada, String origen, String destino, int duracion) {
		this.tipoDeLlamada = tipoLlamada;
		this.origen= origen;
		this.destino= destino;
		this.duracion = duracion;
	}

	public String getTipoDeLlamada() {
		return tipoDeLlamada;
	}

	public String getRemitente() {
		return destino;
	}

	public int getDuracion() {
		return this.duracion;
	}

	public String getOrigen() {
		return origen;
	}

	private double precioPorTipo(){
		if (this.getTipoDeLlamada() == "nacional") {
			// el precio es de 3 pesos por segundo más IVA sin adicional por establecer la llamada
			auxc += this.getDuracion() * 3 + (this.getDuracion() * 3 * 0.21);
		} else if (this.getTipoDeLlamada() == "internacional") {
			// el precio es de 150 pesos por segundo más IVA más 50 pesos por establecer la llamada
			auxc += this.getDuracion() * 150 + (this.getDuracion() * 150 * 0.21) + 50;
		}
	}

	private double descuentoPorTipoPersona(double descuentoFis, double descuentoJur){
		if (cliente.getTipo() == "fisica") {
				auxc -= auxc*descuentoFis;
			} else if(cliente.getTipo() == "juridica") {
				auxc -= auxc*descuentoJur;
			}
	}

	public double calcularMonto(Cliente cliente, double descuentoFis, double descuentoJur){
		double auxc = 0;
		auxc += this.precioPorTipo();
		auxc -= this.descuentoPorTipoPersona(descuentoFis, descuentoJus);
		return auxc;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.TreeSet;
import java.util.ArrayList;
import java.util.Random;
import java.util.SortedSet;

public class GestorNumerosDisponibles {
	private SortedSet<String> lineas = new TreeSet<String>();
	private String tipoGenerador = "ultimo";

	public SortedSet<String> getLineas() {
		return lineas;
	}

	public String obtenerNumeroLibre() {
		String linea;
		switch (tipoGenerador) {
			case "ultimo":
				linea = lineas.last();
				lineas.remove(linea);
				return linea;
			case "primero":
				linea = lineas.first();
				lineas.remove(linea);
				return linea;
			case "random":
				linea = new ArrayList<String>(lineas)
						.get(new Random().nextInt(lineas.size()));
				lineas.remove(linea);
				return linea;
		}
		return null;
	}

	public void cambiarTipoGenerador(String valor) {
		this.tipoGenerador = valor;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Empresa {
	private List<Cliente> clientes = new ArrayList<Cliente>();
	private List<Llamada> llamadas = new ArrayList<Llamada>();
	private GestorNumerosDisponibles guia = new GestorNumerosDisponibles();

	static double descuentoJur = 0.15;
	static double descuentoFis = 0;

	public boolean agregarNumeroTelefono(String str) {
		boolean encontre = guia.getLineas().contains(str);
		if (!encontre) {
			guia.getLineas().add(str);
			encontre= true;
			return encontre;
		}
		else {
			encontre= false;
			return encontre;
		}
	}

	public String obtenerNumeroLibre() {
		return guia.obtenerNumeroLibre();
	}

	public Cliente registrarUsuario(String data, String nombre, String tipo) {
		Cliente var = new Cliente();
		if (tipo.equals("fisica")) {
			var.setNombre(nombre);
			String tel = this.obtenerNumeroLibre();
			var.setTipo(tipo);
			var.setNumeroTelefono(tel);
			var.setDNI(data);
		}
		else if (tipo.equals("juridica")) {
			String tel = this.obtenerNumeroLibre();
			var.setNombre(nombre);
			var.setTipo(tipo);
			var.setNumeroTelefono(tel);
			var.setCuit(data);
		}
		clientes.add(var);
		return var;
	}

	public Llamada registrarLlamada(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new Llamada(t, origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public double calcularMontoTotalLlamadas(Cliente cliente) {
		double c = 0;
		for (Llamada l : cliente.llamadas) {
			c += l.calcularMonto(cliente);
		}
		return ;
	}

	public int cantidadDeUsuarios() {
		return clientes.size();
	}

	public boolean existeUsuario(Cliente persona) {
		return clientes.contains(persona);
	}

	public GestorNumerosDisponibles getGestorNumeros() {
		return this.guia;
	}
}

package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Cliente {
	public List<Llamada> llamadas = new ArrayList<Llamada>();
	private String tipo;
	private String nombre;
	private String numeroTelefono;
	private String cuit;
	private String dni;

	public String getTipo() {
		return tipo;
	}
	public void setTipo(String tipo) {
		this.tipo = tipo;
	}
	public String getNombre() {
		return nombre;
	}
	public void setNombre(String nombre) {
		this.nombre = nombre;
	}
	public String getNumeroTelefono() {
		return numeroTelefono;
	}
	public void setNumeroTelefono(String numeroTelefono) {
		this.numeroTelefono = numeroTelefono;
	}
	public String getCuit() {
		return cuit;
	}
	public void setCuit(String cuit) {
		this.cuit = cuit;
	}
	public String getDNI() {
		return dni;
	}
	public void setDNI(String dni) {
		this.dni = dni;
	}
}
```


## Bad smell: 
* Temporary variables. En el metodo calcularCosto de Llamada, se usan variables auxiliares

### Refactoring: Inline variable
```java
package ar.edu.unlp.info.oo2.facturacion_llamadas;

public class Llamada {
	private String tipoDeLlamada;
	private String origen;
	private String destino;
	private int duracion;

	public Llamada(String tipoLlamada, String origen, String destino, int duracion) {
		this.tipoDeLlamada = tipoLlamada;
		this.origen= origen;
		this.destino= destino;
		this.duracion = duracion;
	}

	public String getTipoDeLlamada() {
		return tipoDeLlamada;
	}

	public String getRemitente() {
		return destino;
	}

	public int getDuracion() {
		return this.duracion;
	}

	public String getOrigen() {
		return origen;
	}

	private double precioPorTipo(){
		if (this.getTipoDeLlamada() == "nacional") {
			// el precio es de 3 pesos por segundo más IVA sin adicional por establecer la llamada
			return this.getDuracion() * 3 + (this.getDuracion() * 3 * 0.21);
		} else if (this.getTipoDeLlamada() == "internacional") {
			// el precio es de 150 pesos por segundo más IVA más 50 pesos por establecer la llamada
			return this.getDuracion() * 150 + (this.getDuracion() * 150 * 0.21) + 50;
		}
	}

	private double descuentoPorTipoPersona(double descuentoFis, double descuentoJur){
		if (cliente.getTipo() == "fisica") {
			auxc -= auxc*descuentoFis;
		} else if(cliente.getTipo() == "juridica") {
			auxc -= auxc*descuentoJur;
		}
	}

	public double calcularMonto(Cliente cliente, double descuentoFis, double descuentoJur){
		return this.precioPorTipo() + this.descuentoPorTipoPersona(descuentoFis, descuentoJus);;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.TreeSet;
import java.util.ArrayList;
import java.util.Random;
import java.util.SortedSet;

public class GestorNumerosDisponibles {
	private SortedSet<String> lineas = new TreeSet<String>();
	private String tipoGenerador = "ultimo";

	public SortedSet<String> getLineas() {
		return lineas;
	}

	public String obtenerNumeroLibre() {
		String linea;
		switch (tipoGenerador) {
			case "ultimo":
				linea = lineas.last();
				lineas.remove(linea);
				return linea;
			case "primero":
				linea = lineas.first();
				lineas.remove(linea);
				return linea;
			case "random":
				linea = new ArrayList<String>(lineas)
						.get(new Random().nextInt(lineas.size()));
				lineas.remove(linea);
				return linea;
		}
		return null;
	}

	public void cambiarTipoGenerador(String valor) {
		this.tipoGenerador = valor;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Empresa {
	private List<Cliente> clientes = new ArrayList<Cliente>();
	private List<Llamada> llamadas = new ArrayList<Llamada>();
	private GestorNumerosDisponibles guia = new GestorNumerosDisponibles();

	static double descuentoJur = 0.15;
	static double descuentoFis = 0;

	public boolean agregarNumeroTelefono(String str) {
		boolean encontre = guia.getLineas().contains(str);
		if (!encontre) {
			guia.getLineas().add(str);
			encontre= true;
			return encontre;
		}
		else {
			encontre= false;
			return encontre;
		}
	}

	public String obtenerNumeroLibre() {
		return guia.obtenerNumeroLibre();
	}

	public Cliente registrarUsuario(String data, String nombre, String tipo) {
		Cliente var = new Cliente();
		if (tipo.equals("fisica")) {
			var.setNombre(nombre);
			String tel = this.obtenerNumeroLibre();
			var.setTipo(tipo);
			var.setNumeroTelefono(tel);
			var.setDNI(data);
		}
		else if (tipo.equals("juridica")) {
			String tel = this.obtenerNumeroLibre();
			var.setNombre(nombre);
			var.setTipo(tipo);
			var.setNumeroTelefono(tel);
			var.setCuit(data);
		}
		clientes.add(var);
		return var;
	}

	public Llamada registrarLlamada(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new Llamada(t, origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public double calcularMontoTotalLlamadas(Cliente cliente) {
		double c = 0;
		for (Llamada l : cliente.llamadas) {
			c += l.calcularMonto(cliente);
		}
		return ;
	}

	public int cantidadDeUsuarios() {
		return clientes.size();
	}

	public boolean existeUsuario(Cliente persona) {
		return clientes.contains(persona);
	}

	public GestorNumerosDisponibles getGestorNumeros() {
		return this.guia;
	}
}

package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Cliente {
	public List<Llamada> llamadas = new ArrayList<Llamada>();
	private String tipo;
	private String nombre;
	private String numeroTelefono;
	private String cuit;
	private String dni;

	public String getTipo() {
		return tipo;
	}
	public void setTipo(String tipo) {
		this.tipo = tipo;
	}
	public String getNombre() {
		return nombre;
	}
	public void setNombre(String nombre) {
		this.nombre = nombre;
	}
	public String getNumeroTelefono() {
		return numeroTelefono;
	}
	public void setNumeroTelefono(String numeroTelefono) {
		this.numeroTelefono = numeroTelefono;
	}
	public String getCuit() {
		return cuit;
	}
	public void setCuit(String cuit) {
		this.cuit = cuit;
	}
	public String getDNI() {
		return dni;
	}
	public void setDNI(String dni) {
		this.dni = dni;
	}
}
```


## Bad smell: 
* Type Conditional. Hay logica condicional en funcion del tipo de llamada

### Refactoring: Replace conditional with Polymorphism
```java
package ar.edu.unlp.info.oo2.facturacion_llamadas;

public abstract class Llamada {
	private String origen;
	private String destino;
	private int duracion;

	public Llamada(String origen, String destino, int duracion) {
		this.origen= origen;
		this.destino= destino;
		this.duracion = duracion;
	}

	public String getTipoDeLlamada() {
		return tipoDeLlamada;
	}

	public String getRemitente() {
		return destino;
	}

	public int getDuracion() {
		return this.duracion;
	}

	public String getOrigen() {
		return origen;
	}

	public abstract double precio();

	private double descuentoPorTipoPersona(double descuentoFis, double descuentoJur){
		if (cliente.getTipo() == "fisica") {
			auxc -= auxc*descuentoFis;
		} else if(cliente.getTipo() == "juridica") {
			auxc -= auxc*descuentoJur;
		}
	}

	public double calcularMonto(Cliente cliente, double descuentoFis, double descuentoJur){
		return this.precio() + this.descuentoPorTipoPersona(descuentoFis, descuentoJus);;
	}
}

public LlamadaNacional extends Llamada{
	@Override
	public public abstract double precio(){
		return this.getDuracion() * 3 + (this.getDuracion() * 3 * 0.21);
	}
}

public LlamadaInternacional extends Llamada{
	@Override
	public public abstract double precio(){
		return this.getDuracion() * 150 + (this.getDuracion() * 150 * 0.21) + 50;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.TreeSet;
import java.util.ArrayList;
import java.util.Random;
import java.util.SortedSet;

public class GestorNumerosDisponibles {
	private SortedSet<String> lineas = new TreeSet<String>();
	private String tipoGenerador = "ultimo";

	public SortedSet<String> getLineas() {
		return lineas;
	}

	public String obtenerNumeroLibre() {
		String linea;
		switch (tipoGenerador) {
			case "ultimo":
				linea = lineas.last();
				lineas.remove(linea);
				return linea;
			case "primero":
				linea = lineas.first();
				lineas.remove(linea);
				return linea;
			case "random":
				linea = new ArrayList<String>(lineas)
						.get(new Random().nextInt(lineas.size()));
				lineas.remove(linea);
				return linea;
		}
		return null;
	}

	public void cambiarTipoGenerador(String valor) {
		this.tipoGenerador = valor;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Empresa {
	private List<Cliente> clientes = new ArrayList<Cliente>();
	private List<Llamada> llamadas = new ArrayList<Llamada>();
	private GestorNumerosDisponibles guia = new GestorNumerosDisponibles();

	static double descuentoJur = 0.15;
	static double descuentoFis = 0;

	public boolean agregarNumeroTelefono(String str) {
		boolean encontre = guia.getLineas().contains(str);
		if (!encontre) {
			guia.getLineas().add(str);
			encontre= true;
			return encontre;
		}
		else {
			encontre= false;
			return encontre;
		}
	}

	public String obtenerNumeroLibre() {
		return guia.obtenerNumeroLibre();
	}

	public Cliente registrarUsuario(String data, String nombre, String tipo) {
		Cliente var = new Cliente();
		if (tipo.equals("fisica")) {
			var.setNombre(nombre);
			String tel = this.obtenerNumeroLibre();
			var.setTipo(tipo);
			var.setNumeroTelefono(tel);
			var.setDNI(data);
		}
		else if (tipo.equals("juridica")) {
			String tel = this.obtenerNumeroLibre();
			var.setNombre(nombre);
			var.setTipo(tipo);
			var.setNumeroTelefono(tel);
			var.setCuit(data);
		}
		clientes.add(var);
		return var;
	}

	public Llamada registrarLlamadaNacional(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new LlamadaNacional(origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public Llamada registrarLlamada(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new LlamadaInternacional(origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public double calcularMontoTotalLlamadas(Cliente cliente) {
		double c = 0;
		for (Llamada l : cliente.llamadas) {
			c += l.calcularMonto(cliente);
		}
		return ;
	}

	public int cantidadDeUsuarios() {
		return clientes.size();
	}

	public boolean existeUsuario(Cliente persona) {
		return clientes.contains(persona);
	}

	public GestorNumerosDisponibles getGestorNumeros() {
		return this.guia;
	}
}

package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Cliente {
	public List<Llamada> llamadas = new ArrayList<Llamada>();
	private String tipo;
	private String nombre;
	private String numeroTelefono;
	private String cuit;
	private String dni;

	public String getTipo() {
		return tipo;
	}
	public void setTipo(String tipo) {
		this.tipo = tipo;
	}
	public String getNombre() {
		return nombre;
	}
	public void setNombre(String nombre) {
		this.nombre = nombre;
	}
	public String getNumeroTelefono() {
		return numeroTelefono;
	}
	public void setNumeroTelefono(String numeroTelefono) {
		this.numeroTelefono = numeroTelefono;
	}
	public String getCuit() {
		return cuit;
	}
	public void setCuit(String cuit) {
		this.cuit = cuit;
	}
	public String getDNI() {
		return dni;
	}
	public void setDNI(String dni) {
		this.dni = dni;
	}
}
```


## Bad smell: 
* Feature envy. Llamada hace calculo de descuento que le correspondería al Cliente

### Refactoring: Move method
```java
package ar.edu.unlp.info.oo2.facturacion_llamadas;

public abstract class Llamada {
	private String origen;
	private String destino;
	private int duracion;

	public Llamada(String origen, String destino, int duracion) {
		this.origen= origen;
		this.destino= destino;
		this.duracion = duracion;
	}

	public String getTipoDeLlamada() {
		return tipoDeLlamada;
	}

	public String getRemitente() {
		return destino;
	}

	public int getDuracion() {
		return this.duracion;
	}

	public String getOrigen() {
		return origen;
	}

	public abstract double precio();

	public double calcularMonto(Cliente cliente, double descuentoFis, double descuentoJur){
		return this.precio() + cliente.calcularDescuento(descuentoFis, descuentoJus);;
	}
}

public LlamadaNacional extends Llamada{
	@Override
	public public abstract double precio(){
		return this.getDuracion() * 3 + (this.getDuracion() * 3 * 0.21);
	}
}

public LlamadaInternacional extends Llamada{
	@Override
	public public abstract double precio(){
		return this.getDuracion() * 150 + (this.getDuracion() * 150 * 0.21) + 50;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.TreeSet;
import java.util.ArrayList;
import java.util.Random;
import java.util.SortedSet;

public class GestorNumerosDisponibles {
	private SortedSet<String> lineas = new TreeSet<String>();
	private String tipoGenerador = "ultimo";

	public SortedSet<String> getLineas() {
		return lineas;
	}

	public String obtenerNumeroLibre() {
		String linea;
		switch (tipoGenerador) {
			case "ultimo":
				linea = lineas.last();
				lineas.remove(linea);
				return linea;
			case "primero":
				linea = lineas.first();
				lineas.remove(linea);
				return linea;
			case "random":
				linea = new ArrayList<String>(lineas)
						.get(new Random().nextInt(lineas.size()));
				lineas.remove(linea);
				return linea;
		}
		return null;
	}

	public void cambiarTipoGenerador(String valor) {
		this.tipoGenerador = valor;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Empresa {
	private List<Cliente> clientes = new ArrayList<Cliente>();
	private List<Llamada> llamadas = new ArrayList<Llamada>();
	private GestorNumerosDisponibles guia = new GestorNumerosDisponibles();

	static double descuentoJur = 0.15;
	static double descuentoFis = 0;

	public boolean agregarNumeroTelefono(String str) {
		boolean encontre = guia.getLineas().contains(str);
		if (!encontre) {
			guia.getLineas().add(str);
			encontre= true;
			return encontre;
		}
		else {
			encontre= false;
			return encontre;
		}
	}

	public String obtenerNumeroLibre() {
		return guia.obtenerNumeroLibre();
	}

	public Cliente registrarUsuario(String data, String nombre, String tipo) {
		Cliente var = new Cliente();
		if (tipo.equals("fisica")) {
			var.setNombre(nombre);
			String tel = this.obtenerNumeroLibre();
			var.setTipo(tipo);
			var.setNumeroTelefono(tel);
			var.setDNI(data);
		}
		else if (tipo.equals("juridica")) {
			String tel = this.obtenerNumeroLibre();
			var.setNombre(nombre);
			var.setTipo(tipo);
			var.setNumeroTelefono(tel);
			var.setCuit(data);
		}
		clientes.add(var);
		return var;
	}

	public Llamada registrarLlamadaNacional(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new LlamadaNacional(origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public Llamada registrarLlamada(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new LlamadaInternacional(origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public double calcularMontoTotalLlamadas(Cliente cliente) {
		double c = 0;
		for (Llamada l : cliente.llamadas) {
			c += l.calcularMonto(cliente);
		}
		return ;
	}

	public int cantidadDeUsuarios() {
		return clientes.size();
	}

	public boolean existeUsuario(Cliente persona) {
		return clientes.contains(persona);
	}

	public GestorNumerosDisponibles getGestorNumeros() {
		return this.guia;
	}
}

package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Cliente {
	public List<Llamada> llamadas = new ArrayList<Llamada>();
	private String tipo;
	private String nombre;
	private String numeroTelefono;
	private String cuit;
	private String dni;

	public String getTipo() {
		return tipo;
	}
	public void setTipo(String tipo) {
		this.tipo = tipo;
	}
	public String getNombre() {
		return nombre;
	}
	public void setNombre(String nombre) {
		this.nombre = nombre;
	}
	public String getNumeroTelefono() {
		return numeroTelefono;
	}
	public void setNumeroTelefono(String numeroTelefono) {
		this.numeroTelefono = numeroTelefono;
	}
	public String getCuit() {
		return cuit;
	}
	public void setCuit(String cuit) {
		this.cuit = cuit;
	}
	public String getDNI() {
		return dni;
	}
	public void setDNI(String dni) {
		this.dni = dni;
	}

	public double calcularDescuento(double descuentoFis, double descuentoJur){
		if (cliente.getTipo() == "fisica") {
			auxc -= auxc*descuentoFis;
		} else if(cliente.getTipo() == "juridica") {
			auxc -= auxc*descuentoJur;
		}
	}

}
```


## Bad smell: 
* Type conditional. El cliente tiene logica condicional en funcion del tipo de persona

### Refactoring: Replace conditional with Strategy
* Tambien apliqué move variable, para las constantes descuentoFis y descuentoJur (tal vez debería haberlo aplicado antes)
* Tuve que separar el metodo registrarUsuario en uno para fisica y otro para juridica
```java
package ar.edu.unlp.info.oo2.facturacion_llamadas;

public abstract class Llamada {
	private String origen;
	private String destino;
	private int duracion;

	public Llamada(String origen, String destino, int duracion) {
		this.origen= origen;
		this.destino= destino;
		this.duracion = duracion;
	}

	public String getTipoDeLlamada() {
		return tipoDeLlamada;
	}

	public String getRemitente() {
		return destino;
	}

	public int getDuracion() {
		return this.duracion;
	}

	public String getOrigen() {
		return origen;
	}

	public abstract double precio();

	public double calcularMonto(Cliente cliente){
		return this.precio() - cliente.calcularDescuento();;
	}
}

public LlamadaNacional extends Llamada{
	@Override
	public public abstract double precio(){
		return this.getDuracion() * 3 + (this.getDuracion() * 3 * 0.21);
	}
}

public LlamadaInternacional extends Llamada{
	@Override
	public public abstract double precio(){
		return this.getDuracion() * 150 + (this.getDuracion() * 150 * 0.21) + 50;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.TreeSet;
import java.util.ArrayList;
import java.util.Random;
import java.util.SortedSet;

public class GestorNumerosDisponibles {
	private SortedSet<String> lineas = new TreeSet<String>();
	private String tipoGenerador = "ultimo";

	public SortedSet<String> getLineas() {
		return lineas;
	}

	public String obtenerNumeroLibre() {
		String linea;
		switch (tipoGenerador) {
			case "ultimo":
				linea = lineas.last();
				lineas.remove(linea);
				return linea;
			case "primero":
				linea = lineas.first();
				lineas.remove(linea);
				return linea;
			case "random":
				linea = new ArrayList<String>(lineas)
						.get(new Random().nextInt(lineas.size()));
				lineas.remove(linea);
				return linea;
		}
		return null;
	}

	public void cambiarTipoGenerador(String valor) {
		this.tipoGenerador = valor;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Empresa {
	private List<Cliente> clientes = new ArrayList<Cliente>();
	private List<Llamada> llamadas = new ArrayList<Llamada>();
	private GestorNumerosDisponibles guia = new GestorNumerosDisponibles();

	public boolean agregarNumeroTelefono(String str) {
		boolean encontre = guia.getLineas().contains(str);
		if (!encontre) {
			guia.getLineas().add(str);
			encontre= true;
			return encontre;
		}
		else {
			encontre= false;
			return encontre;
		}
	}

	public String obtenerNumeroLibre() {
		return guia.obtenerNumeroLibre();
	}

	public Cliente registrarUsuarioFisica(String data, String nombre) {
		Cliente var = new Cliente();
		var.setNombre(nombre);
		String tel = this.obtenerNumeroLibre();
		var.setTipo(new DescuentoFisica);
		var.setNumeroTelefono(tel);
		var.setDNI(data);
		clientes.add(var);
		return var;
	}

	public Cliente registrarUsuarioJuridica(String data, String nombre) {
		Cliente var = new Cliente();
		var.setNombre(nombre);
		String tel = this.obtenerNumeroLibre();
		var.setTipo(new DescuentoJuridica);
		var.setNumeroTelefono(tel);
		var.setDNI(data);
		clientes.add(var);
		return var;
	}

	public Llamada registrarLlamadaNacional(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new LlamadaNacional(origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public Llamada registrarLlamada(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new LlamadaInternacional(origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public double calcularMontoTotalLlamadas(Cliente cliente) {
		double c = 0;
		for (Llamada l : cliente.llamadas) {
			c += l.calcularMonto(cliente);
		}
		return ;
	}

	public int cantidadDeUsuarios() {
		return clientes.size();
	}

	public boolean existeUsuario(Cliente persona) {
		return clientes.contains(persona);
	}

	public GestorNumerosDisponibles getGestorNumeros() {
		return this.guia;
	}
}

package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Cliente {
	public List<Llamada> llamadas = new ArrayList<Llamada>();
	private Descuento tipo;
	private String nombre;
	private String numeroTelefono;
	private String cuit;
	private String dni;

	public Descuento getTipo() {
		return tipo;
	}
	public void setTipo(Descuento tipo) {
		this.tipo = tipo;
	}
	public String getNombre() {
		return nombre;
	}
	public void setNombre(String nombre) {
		this.nombre = nombre;
	}
	public String getNumeroTelefono() {
		return numeroTelefono;
	}
	public void setNumeroTelefono(String numeroTelefono) {
		this.numeroTelefono = numeroTelefono;
	}
	public String getCuit() {
		return cuit;
	}
	public void setCuit(String cuit) {
		this.cuit = cuit;
	}
	public String getDNI() {
		return dni;
	}
	public void setDNI(String dni) {
		this.dni = dni;
	}

	

public abstract class Descuento{
	public abstract double calcularDescuento();
} 

public class DescuentoJuridica extends Descuento{

	private static double descuentoJur = 0.15;

	public double calcularDescuento(){
		return auxc*this.descuentoJur;
	}
}

public class DescuentoFisica extends Descuento{
	private static double descuentoFis = 0.15;

	public double calcularDescuento(){
		return auxc*descuentoFis;
	}
}
```


## Bad smell: 
* Duplicate code. registrarUsuarioFisica y registrarUsuarioJuridica son muy similares

### Refactoring: Form template method
* En realidad, creo que no tenía sentido hacer una superclase de empresa y 2 subclases y todo el quilombo que implica el template method, lo que hice es más un extract method de la parte en comun, y un replace temp with query. Mantiene la interface y reutiliza codigo.
```java
package ar.edu.unlp.info.oo2.facturacion_llamadas;

public abstract class Llamada {
	private String origen;
	private String destino;
	private int duracion;

	public Llamada(String origen, String destino, int duracion) {
		this.origen= origen;
		this.destino= destino;
		this.duracion = duracion;
	}

	public String getTipoDeLlamada() {
		return tipoDeLlamada;
	}

	public String getRemitente() {
		return destino;
	}

	public int getDuracion() {
		return this.duracion;
	}

	public String getOrigen() {
		return origen;
	}

	public abstract double precio();

	public double calcularMonto(Cliente cliente){
		return this.precio() - cliente.calcularDescuento();;
	}
}

public LlamadaNacional extends Llamada{
	@Override
	public public abstract double precio(){
		return this.getDuracion() * 3 + (this.getDuracion() * 3 * 0.21);
	}
}

public LlamadaInternacional extends Llamada{
	@Override
	public public abstract double precio(){
		return this.getDuracion() * 150 + (this.getDuracion() * 150 * 0.21) + 50;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.TreeSet;
import java.util.ArrayList;
import java.util.Random;
import java.util.SortedSet;

public class GestorNumerosDisponibles {
	private SortedSet<String> lineas = new TreeSet<String>();
	private String tipoGenerador = "ultimo";

	public SortedSet<String> getLineas() {
		return lineas;
	}

	public String obtenerNumeroLibre() {
		String linea;
		switch (tipoGenerador) {
			case "ultimo":
				linea = lineas.last();
				lineas.remove(linea);
				return linea;
			case "primero":
				linea = lineas.first();
				lineas.remove(linea);
				return linea;
			case "random":
				linea = new ArrayList<String>(lineas)
						.get(new Random().nextInt(lineas.size()));
				lineas.remove(linea);
				return linea;
		}
		return null;
	}

	public void cambiarTipoGenerador(String valor) {
		this.tipoGenerador = valor;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Empresa {
	private List<Cliente> clientes = new ArrayList<Cliente>();
	private List<Llamada> llamadas = new ArrayList<Llamada>();
	private GestorNumerosDisponibles guia = new GestorNumerosDisponibles();

	public boolean agregarNumeroTelefono(String str) {
		boolean encontre = guia.getLineas().contains(str);
		if (!encontre) {
			guia.getLineas().add(str);
			encontre= true;
			return encontre;
		}
		else {
			encontre= false;
			return encontre;
		}
	}

	public String obtenerNumeroLibre() {
		return guia.obtenerNumeroLibre();
	}

	private Cliente registrarUsuarioTemplate(String data, String nombre){
		Cliente var = new Cliente();
		var.setNombre(nombre);
		String tel = this.obtenerNumeroLibre();
		var.setTipo(new DescuentoFisica);
		var.setNumeroTelefono(tel);
		clientes.add(var);
		return var;
	}

	public Cliente registrarUsuarioFisica(String data, String nombre) {
		Cliente var = this.registrarUsuarioTemplate(data, nombre);
		var.setDNI(data);
		return var;
	}

	public Cliente registrarUsuarioJuridica(String data, String nombre) {
		Cliente var = this.registrarUsuarioTemplate(data, nombre);
		var.setCUIT(data);
		return var;
	}

	public Llamada registrarLlamadaNacional(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new LlamadaNacional(origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public Llamada registrarLlamada(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new LlamadaInternacional(origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public double calcularMontoTotalLlamadas(Cliente cliente) {
		double c = 0;
		for (Llamada l : cliente.llamadas) {
			c += l.calcularMonto(cliente);
		}
		return ;
	}

	public int cantidadDeUsuarios() {
		return clientes.size();
	}

	public boolean existeUsuario(Cliente persona) {
		return clientes.contains(persona);
	}

	public GestorNumerosDisponibles getGestorNumeros() {
		return this.guia;
	}
}

package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Cliente {
	public List<Llamada> llamadas = new ArrayList<Llamada>();
	private Descuento tipo;
	private String nombre;
	private String numeroTelefono;
	private String cuit;
	private String dni;

	public Descuento getTipo() {
		return tipo;
	}
	public void setTipo(Descuento tipo) {
		this.tipo = tipo;
	}
	public String getNombre() {
		return nombre;
	}
	public void setNombre(String nombre) {
		this.nombre = nombre;
	}
	public String getNumeroTelefono() {
		return numeroTelefono;
	}
	public void setNumeroTelefono(String numeroTelefono) {
		this.numeroTelefono = numeroTelefono;
	}
	public String getCuit() {
		return cuit;
	}
	public void setCuit(String cuit) {
		this.cuit = cuit;
	}
	public String getDNI() {
		return dni;
	}
	public void setDNI(String dni) {
		this.dni = dni;
	}

	

public abstract class Descuento{
	public abstract double calcularDescuento();
} 

public class DescuentoJuridica extends Descuento{

	private static double descuentoJur = 0.15;

	public double calcularDescuento(){
		return auxc*this.descuentoJur;
	}
}

public class DescuentoFisica extends Descuento{
	private static double descuentoFis = 0.15;

	public double calcularDescuento(){
		return auxc*descuentoFis;
	}
}
```


## Bad smell: 
* Reinventar la rueda. Hace un for con acumulador en calcularMontoTotalLlamadas

### Refactoring: Replace loop with pipeline
```java
package ar.edu.unlp.info.oo2.facturacion_llamadas;

public abstract class Llamada {
	private String origen;
	private String destino;
	private int duracion;

	public Llamada(String origen, String destino, int duracion) {
		this.origen= origen;
		this.destino= destino;
		this.duracion = duracion;
	}

	public String getTipoDeLlamada() {
		return tipoDeLlamada;
	}

	public String getRemitente() {
		return destino;
	}

	public int getDuracion() {
		return this.duracion;
	}

	public String getOrigen() {
		return origen;
	}

	public abstract double precio();

	public double calcularMonto(Cliente cliente){
		return this.precio() - cliente.calcularDescuento();;
	}
}

public LlamadaNacional extends Llamada{
	@Override
	public public abstract double precio(){
		return this.getDuracion() * 3 + (this.getDuracion() * 3 * 0.21);
	}
}

public LlamadaInternacional extends Llamada{
	@Override
	public public abstract double precio(){
		return this.getDuracion() * 150 + (this.getDuracion() * 150 * 0.21) + 50;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.TreeSet;
import java.util.ArrayList;
import java.util.Random;
import java.util.SortedSet;

public class GestorNumerosDisponibles {
	private SortedSet<String> lineas = new TreeSet<String>();
	private String tipoGenerador = "ultimo";

	public SortedSet<String> getLineas() {
		return lineas;
	}

	public String obtenerNumeroLibre() {
		String linea;
		switch (tipoGenerador) {
			case "ultimo":
				linea = lineas.last();
				lineas.remove(linea);
				return linea;
			case "primero":
				linea = lineas.first();
				lineas.remove(linea);
				return linea;
			case "random":
				linea = new ArrayList<String>(lineas)
						.get(new Random().nextInt(lineas.size()));
				lineas.remove(linea);
				return linea;
		}
		return null;
	}

	public void cambiarTipoGenerador(String valor) {
		this.tipoGenerador = valor;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Empresa {
	private List<Cliente> clientes = new ArrayList<Cliente>();
	private List<Llamada> llamadas = new ArrayList<Llamada>();
	private GestorNumerosDisponibles guia = new GestorNumerosDisponibles();

	public boolean agregarNumeroTelefono(String str) {
		boolean encontre = guia.getLineas().contains(str);
		if (!encontre) {
			guia.getLineas().add(str);
			encontre= true;
			return encontre;
		}
		else {
			encontre= false;
			return encontre;
		}
	}

	public String obtenerNumeroLibre() {
		return guia.obtenerNumeroLibre();
	}

	private Cliente registrarUsuarioTemplate(String data, String nombre){
		Cliente var = new Cliente();
		var.setNombre(nombre);
		String tel = this.obtenerNumeroLibre();
		var.setTipo(new DescuentoFisica);
		var.setNumeroTelefono(tel);
		clientes.add(var);
		return var;
	}

	public Cliente registrarUsuarioFisica(String data, String nombre) {
		Cliente var = this.registrarUsuarioTemplate(data, nombre);
		var.setDNI(data);
		return var;
	}

	public Cliente registrarUsuarioJuridica(String data, String nombre) {
		Cliente var = this.registrarUsuarioTemplate(data, nombre);
		var.setCUIT(data);
		return var;
	}

	public Llamada registrarLlamadaNacional(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new LlamadaNacional(origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public Llamada registrarLlamadaInternacional(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new LlamadaInternacional(origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public double calcularMontoTotalLlamadas(Cliente cliente) {
    return cliente.getLlamadas().stream()
        .mapToDouble(l -> l.calcularMonto(cliente))
        .sum();
	}


	public int cantidadDeUsuarios() {
		return clientes.size();
	}

	public boolean existeUsuario(Cliente persona) {
		return clientes.contains(persona);
	}

	public GestorNumerosDisponibles getGestorNumeros() {
		return this.guia;
	}
}

package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Cliente {
	public List<Llamada> llamadas = new ArrayList<Llamada>();
	private Descuento tipo;
	private String nombre;
	private String numeroTelefono;
	private String cuit;
	private String dni;

	public Descuento getTipo() {
		return tipo;
	}
	public void setTipo(Descuento tipo) {
		this.tipo = tipo;
	}
	public String getNombre() {
		return nombre;
	}
	public void setNombre(String nombre) {
		this.nombre = nombre;
	}
	public String getNumeroTelefono() {
		return numeroTelefono;
	}
	public void setNumeroTelefono(String numeroTelefono) {
		this.numeroTelefono = numeroTelefono;
	}
	public String getCuit() {
		return cuit;
	}
	public void setCuit(String cuit) {
		this.cuit = cuit;
	}
	public String getDNI() {
		return dni;
	}
	public void setDNI(String dni) {
		this.dni = dni;
	}

	

public abstract class Descuento{
	public abstract double calcularDescuento();
} 

public class DescuentoJuridica extends Descuento{

	private static double descuentoJur = 0.15;

	public double calcularDescuento(){
		return auxc*this.descuentoJur;
	}
}

public class DescuentoFisica extends Descuento{
	private static double descuentoFis = 0.15;

	public double calcularDescuento(){
		return auxc*descuentoFis;
	}
}
```


## Bad smell: 
* Duplicate returns (no se si existe eso jasdj) en metodo agregarNumeroTelefono

### Refactoring: Consolidate conditional 
```java
package ar.edu.unlp.info.oo2.facturacion_llamadas;

public abstract class Llamada {
	private String origen;
	private String destino;
	private int duracion;

	public Llamada(String origen, String destino, int duracion) {
		this.origen= origen;
		this.destino= destino;
		this.duracion = duracion;
	}

	public String getTipoDeLlamada() {
		return tipoDeLlamada;
	}

	public String getRemitente() {
		return destino;
	}

	public int getDuracion() {
		return this.duracion;
	}

	public String getOrigen() {
		return origen;
	}

	public abstract double precio();

	public double calcularMonto(Cliente cliente){
		return this.precio() - cliente.calcularDescuento();;
	}
}

public LlamadaNacional extends Llamada{
	@Override
	public public abstract double precio(){
		return this.getDuracion() * 3 + (this.getDuracion() * 3 * 0.21);
	}
}

public LlamadaInternacional extends Llamada{
	@Override
	public public abstract double precio(){
		return this.getDuracion() * 150 + (this.getDuracion() * 150 * 0.21) + 50;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.TreeSet;
import java.util.ArrayList;
import java.util.Random;
import java.util.SortedSet;

public class GestorNumerosDisponibles {
	private SortedSet<String> lineas = new TreeSet<String>();
	private String tipoGenerador = "ultimo";

	public SortedSet<String> getLineas() {
		return lineas;
	}

	public String obtenerNumeroLibre() {
		String linea;
		switch (tipoGenerador) {
			case "ultimo":
				linea = lineas.last();
				lineas.remove(linea);
				return linea;
			case "primero":
				linea = lineas.first();
				lineas.remove(linea);
				return linea;
			case "random":
				linea = new ArrayList<String>(lineas)
						.get(new Random().nextInt(lineas.size()));
				lineas.remove(linea);
				return linea;
		}
		return null;
	}

	public void cambiarTipoGenerador(String valor) {
		this.tipoGenerador = valor;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Empresa {
	private List<Cliente> clientes = new ArrayList<Cliente>();
	private List<Llamada> llamadas = new ArrayList<Llamada>();
	private GestorNumerosDisponibles guia = new GestorNumerosDisponibles();

	private boolean isNumeroRepetido(String numero) {
		return this.getLineas().contains(numero);
	}
	
	public boolean agregarNumeroTelefono(String str) {
		if (!this.isNumeroRepetido(str)) {
			this.getLineas().add(str);
			return true;
		}
		else {
			return false;
		}
	}

	public String obtenerNumeroLibre() {
		return guia.obtenerNumeroLibre();
	}

	private Cliente registrarUsuarioTemplate(String data, String nombre){
		Cliente var = new Cliente();
		var.setNombre(nombre);
		String tel = this.obtenerNumeroLibre();
		var.setTipo(new DescuentoFisica);
		var.setNumeroTelefono(tel);
		clientes.add(var);
		return var;
	}

	public Cliente registrarUsuarioFisica(String data, String nombre) {
		Cliente var = this.registrarUsuarioTemplate(data, nombre);
		var.setDNI(data);
		return var;
	}

	public Cliente registrarUsuarioJuridica(String data, String nombre) {
		Cliente var = this.registrarUsuarioTemplate(data, nombre);
		var.setCUIT(data);
		return var;
	}

	public Llamada registrarLlamadaNacional(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new LlamadaNacional(origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public Llamada registrarLlamadaInternacional(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new LlamadaInternacional(origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public double calcularMontoTotalLlamadas(Cliente cliente) {
    return cliente.getLlamadas().stream()
        .mapToDouble(l -> l.calcularMonto(cliente))
        .sum();
	}


	public int cantidadDeUsuarios() {
		return clientes.size();
	}

	public boolean existeUsuario(Cliente persona) {
		return clientes.contains(persona);
	}

	public GestorNumerosDisponibles getGestorNumeros() {
		return this.guia;
	}
}

package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Cliente {
	public List<Llamada> llamadas = new ArrayList<Llamada>();
	private Descuento tipo;
	private String nombre;
	private String numeroTelefono;
	private String cuit;
	private String dni;

	public Descuento getTipo() {
		return tipo;
	}
	public void setTipo(Descuento tipo) {
		this.tipo = tipo;
	}
	public String getNombre() {
		return nombre;
	}
	public void setNombre(String nombre) {
		this.nombre = nombre;
	}
	public String getNumeroTelefono() {
		return numeroTelefono;
	}
	public void setNumeroTelefono(String numeroTelefono) {
		this.numeroTelefono = numeroTelefono;
	}
	public String getCuit() {
		return cuit;
	}
	public void setCuit(String cuit) {
		this.cuit = cuit;
	}
	public String getDNI() {
		return dni;
	}
	public void setDNI(String dni) {
		this.dni = dni;
	}

	

public abstract class Descuento{
	public abstract double calcularDescuento();
} 

public class DescuentoJuridica extends Descuento{

	private static double descuentoJur = 0.15;

	public double calcularDescuento(){
		return auxc*this.descuentoJur;
	}
}

public class DescuentoFisica extends Descuento{
	private static double descuentoFis = 0.15;

	public double calcularDescuento(){
		return auxc*descuentoFis;
	}
}
```


## Bad smell: 
* Switch statement y type code en clase GeneradorNumero

### Refactoring: Replace conditional Logic with Strategy 
```java
package ar.edu.unlp.info.oo2.facturacion_llamadas;

public abstract class Llamada {
	private String origen;
	private String destino;
	private int duracion;

	public Llamada(String origen, String destino, int duracion) {
		this.origen= origen;
		this.destino= destino;
		this.duracion = duracion;
	}

	public String getTipoDeLlamada() {
		return tipoDeLlamada;
	}

	public String getRemitente() {
		return destino;
	}

	public int getDuracion() {
		return this.duracion;
	}

	public String getOrigen() {
		return origen;
	}

	public abstract double precio();

	public double calcularMonto(Cliente cliente){
		return this.precio() - cliente.calcularDescuento();;
	}
}

public LlamadaNacional extends Llamada{
	@Override
	public public abstract double precio(){
		return this.getDuracion() * 3 + (this.getDuracion() * 3 * 0.21);
	}
}

public LlamadaInternacional extends Llamada{
	@Override
	public public abstract double precio(){
		return this.getDuracion() * 150 + (this.getDuracion() * 150 * 0.21) + 50;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.TreeSet;
import java.util.ArrayList;
import java.util.Random;
import java.util.SortedSet;

public interface GeneradorNumero {
    String obtenerNumeroLibre(SortedSet<String> lineas);
}


public class GeneradorUltimo implements GeneradorNumero {
    @Override
    public String obtenerNumeroLibre(SortedSet<String> lineas) {
        String linea = lineas.last();
        lineas.remove(linea);
        return linea;
    }
}

public class GeneradorPrimero implements GeneradorNumero {
    @Override
    public String obtenerNumeroLibre(SortedSet<String> lineas) {
        String linea = lineas.first();
        lineas.remove(linea);
        return linea;
    }
}

public class GeneradorRandom implements GeneradorNumero {
    private final Random rnd = new Random();
    @Override
    public String obtenerNumeroLibre(SortedSet<String> lineas) {
        List<String> copia = new ArrayList<>(lineas);
        String linea = copia.get(rnd.nextInt(copia.size()));
        lineas.remove(linea);
        return linea;
    }
}


public class GestorNumerosDisponibles {
	private SortedSet<String> lineas = new TreeSet<String>();
	private String tipoGenerador =  new GeneradorUltimo();

	public SortedSet<String> getLineas() {
		return lineas;
	}

	public String obtenerNumeroLibre() {
		return generador.obtenerNumeroLibre(lineas);
	}

	public void cambiarTipoGenerador(GeneradorNumero gen) {
		this.generador = gen;
	}
}


package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Empresa {
	private List<Cliente> clientes = new ArrayList<Cliente>();
	private List<Llamada> llamadas = new ArrayList<Llamada>();
	private GestorNumerosDisponibles guia = new GestorNumerosDisponibles();

	private boolean isNumeroRepetido(String numero) {
		return this.getLineas().contains(numero);
	}
	
	public boolean agregarNumeroTelefono(String str) {
		if (!this.isNumeroRepetido(str)) {
			this.getLineas().add(str);
			return true;
		}
		else {
			return false;
		}
	}

	public String obtenerNumeroLibre() {
		return guia.obtenerNumeroLibre();
	}

	private Cliente registrarUsuarioTemplate(String data, String nombre){
		Cliente var = new Cliente();
		var.setNombre(nombre);
		String tel = this.obtenerNumeroLibre();
		var.setTipo(new DescuentoFisica);
		var.setNumeroTelefono(tel);
		clientes.add(var);
		return var;
	}

	public Cliente registrarUsuarioFisica(String data, String nombre) {
		Cliente var = this.registrarUsuarioTemplate(data, nombre);
		var.setDNI(data);
		return var;
	}

	public Cliente registrarUsuarioJuridica(String data, String nombre) {
		Cliente var = this.registrarUsuarioTemplate(data, nombre);
		var.setCUIT(data);
		return var;
	}

	public Llamada registrarLlamadaNacional(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new LlamadaNacional(origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public Llamada registrarLlamadaInternacional(Cliente origen, Cliente destino, String t, int duracion) {
		Llamada llamada = new LlamadaInternacional(origen.getNumeroTelefono(), destino.getNumeroTelefono(), duracion);
		llamadas.add(llamada);
		origen.llamadas.add(llamada);
		return llamada;
	}

	public double calcularMontoTotalLlamadas(Cliente cliente) {
    return cliente.getLlamadas().stream()
        .mapToDouble(l -> l.calcularMonto(cliente))
        .sum();
	}


	public int cantidadDeUsuarios() {
		return clientes.size();
	}

	public boolean existeUsuario(Cliente persona) {
		return clientes.contains(persona);
	}

	public GestorNumerosDisponibles getGestorNumeros() {
		return this.guia;
	}
}

package ar.edu.unlp.info.oo2.facturacion_llamadas;

import java.util.ArrayList;
import java.util.List;

public class Cliente {
	public List<Llamada> llamadas = new ArrayList<Llamada>();
	private Descuento tipo;
	private String nombre;
	private String numeroTelefono;
	private String cuit;
	private String dni;

	public Descuento getTipo() {
		return tipo;
	}
	public void setTipo(Descuento tipo) {
		this.tipo = tipo;
	}
	public String getNombre() {
		return nombre;
	}
	public void setNombre(String nombre) {
		this.nombre = nombre;
	}
	public String getNumeroTelefono() {
		return numeroTelefono;
	}
	public void setNumeroTelefono(String numeroTelefono) {
		this.numeroTelefono = numeroTelefono;
	}
	public String getCuit() {
		return cuit;
	}
	public void setCuit(String cuit) {
		this.cuit = cuit;
	}
	public String getDNI() {
		return dni;
	}
	public void setDNI(String dni) {
		this.dni = dni;
	}

	

public abstract class Descuento{
	public abstract double calcularDescuento();
} 

public class DescuentoJuridica extends Descuento{

	private static double descuentoJur = 0.15;

	public double calcularDescuento(){
		return auxc*this.descuentoJur;
	}
}

public class DescuentoFisica extends Descuento{
	private static double descuentoFis = 0.15;

	public double calcularDescuento(){
		return auxc*descuentoFis;
	}
}
```

## Bad smell: 
* Nombres feos

### Refactoring: Rename method, rename variable y rename field a todo, por todos lados. 
```java
	// ni ganas, pero mirá a mi perro
```
<img src="uml.jpg" alt="cooper" width="400"/>

