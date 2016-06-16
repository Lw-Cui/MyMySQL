# MyMySQL
It's MySQL -- My own Structured Query Language.

## Syntax
```
(productNo, productName) @Product @OrderDetail 
		[(productNo) @Product [productPrice >= 10] & [productName != 'DRAM']]
		| [quantity >= 3] & [quantity < 5]
```

## Parse
```
 SELECT Product.productNo, Product.productName
 FROM Product INNER JOIN OrderDetail ON Product.productNo = OrderDetail.productNo
 WHERE (EXISTS ( SELECT Product.productNo
 FROM Product WHERE (Product.productPrice >= 10  and Product.productName != 'DRAM' )
 ) or (OrderDetail.quantity >= 3  and OrderDetail.quantity < 5 ))
```

## Result
```
P20050001      32M DRAM
P20050002      17 Display
P20050003      120GB
P20050005      Keyboard
P20060001      VGA
P20060002      Net card
P20060003      Pentium100CPU
P20070001      1G DDR
P20070002      52 CD disk
P20070003      Dictionary
P20070004      9600bits/s Mordern
P20080001      PentiumMotherboard
P20080002      SL-K8AMotherboard
P20080003      777FT Display
```
