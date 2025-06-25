# Ejercicio 1.1
```java
/** 
* Retorna el límite de crédito del cliente
*/
public double lmtCrdt() {...

/** 
* Retorna el monto facturado al cliente desde la fecha f1 a la fecha f2
*/
protected double mtFcE(LocalDate f1, LocalDate f2) {...

/** 
* Retorna el monto cobrado al cliente desde la fecha f1 a la fecha f2
*/
private double mtCbE(LocalDate f1, LocalDate f2) {...
```

## Bad smell: Mal nombre
## Refactoring: Rename method
```java
/** 
* Retorna el límite de crédito del cliente
*/
public double getLimiteCredito() {...

/** 
* Retorna el monto facturado al cliente desde la fecha f1 a la fecha f2
*/
protected double mtFcE(LocalDate f1, LocalDate f2) {...

/** 
* Retorna el monto cobrado al cliente desde la fecha f1 a la fecha f2
*/
private double mtCbE(LocalDate f1, LocalDate f2) {...

```

## Bad smell: Mal nombre
## Refactoring: Rename method
```java
/** 
* Retorna el límite de crédito del cliente
*/
public double getLimiteCredito() {...

/** 
* Retorna el monto facturado al cliente desde la fecha f1 a la fecha f2
*/
protected double getMontoFacturado(LocalDate f1, LocalDate f2) {...

/** 
* Retorna el monto cobrado al cliente desde la fecha f1 a la fecha f2
*/
private double mtCbE(LocalDate f1, LocalDate f2) {...

```

## Bad smell: Mal nombre
## Refactoring: Rename method
```java
/** 
* Retorna el límite de crédito del cliente
*/
public double getLimiteCredito() {...

/** 
* Retorna el monto facturado al cliente desde la fecha f1 a la fecha f2
*/
protected double getMontoFacturado(LocalDate f1, LocalDate f2) {...

/** 
* Retorna el monto cobrado al cliente desde la fecha f1 a la fecha f2
*/
private double getMontoCobrado(LocalDate f1, LocalDate f2) {...

```