drop database OrderDB;
create database OrderDB;
use OrderDB;

create table Employee(employeeNo char(8) primary key, EmployeeName varchar(10),
	gender char(1), birthday Datetime, address varchar(50),
	telephone varchar(20), hireDate Datetime, department varchar(30),
	headShip varchar(10), salary numeric);

create table Product(productNo char(9) primary key,
	productName varchar(40), productClass varchar(20),
	productPrice numeric(7, 2), inStock int);

create table Customer(customerNo char(9) primary key, customerName varchar(40), 
	telephone varchar(20), address varchar(40), zip char(6));

create table OrderMaster(orderNo char(12) primary key,
	customerNo char(9), foreign key (customerNo) references Customer(customerNo),
	employeeNo char(8), foreign key(employeeNo) references Employee(employeeNo),
	orderDate Datetime, oderSum Numeric(9.2), invoiceNo char(10));

create table OrderDetail(orderNo char(12), foreign key (orderNo) references OrderMaster(orderNo),
	productNo char(9), foreign key (productNo) references Product(productNo),
	quantity int, price numeric(7, 2));


INSERT Customer VALUES ('C20050001', 'Tongyi', '022-3566021', 'Tianjing', '220012');
INSERT Customer VALUES ('C20050002', 'Xinglong', '022-3562452', 'Tianjing', '220301');
INSERT Customer VALUES ('C20050003', 'Shengwu', '010-2121000', 'Beijing', '108001');
INSERT Customer VALUES ('C20050004', 'Wuyi', '021-4532187', 'Shanghai', '210100');
INSERT Customer VALUES ('C20060001', 'Dadi', '010-1165152', 'Beijing', '100803');
INSERT Customer VALUES ('C20060002', 'Lianhe', '021-4568451', 'Shanghai', '210100');
INSERT Customer VALUES ('C20070001', 'Nanchang Research', '0791-4412152', 'NanChange', '330046');
INSERT Customer VALUES ('C20070002', 'World', '021-4564512', 'Shanghai', '210230');
INSERT Customer VALUES ('C20070003', 'Wanda', '022-4533141', 'Tianjing', '220400');
INSERT Customer VALUES ('C20080001', 'Hongdu', '010-5421585', 'Beijing', '100800');

INSERT Employee VALUES ('E2005001', 'Zhiqian Yu', 'M', CAST(0x00005D2500000000 AS Datetime), 'Qing hai 18#', '13817605008', CAST(0x0000808C00000000 AS Datetime), 'Finance Section', 'Chef', CAST(5800 AS decimal(18, 0)));
INSERT Employee VALUES ('E2005002', 'Xiaomei', 'F', CAST(0x0000695700000000 AS Datetime), 'Shanghai 8#', '13607405016', CAST(0x0000822B00000000 AS Datetime), 'Bussiness Section', 'Staff', CAST(2400 AS decimal(18, 0)));
INSERT Employee VALUES ('E2005003', 'Xiaojuan', 'F', CAST(0x0000686700000000 AS Datetime), 'Shanghai 66#', '13707305025', CAST(0x0000839900000000 AS Datetime), 'Bussiness Section', 'Staff', CAST(2600 AS decimal(18, 0)));
INSERT Employee VALUES ('E2005004', 'Zhanglu', 'F', CAST(0x00005F9B00000000 AS Datetime), 'Nanchang 13#', '15907205134', CAST(0x000080BE00000000 AS Datetime), 'Bussiness Section', 'Chef', CAST(4100 AS decimal(18, 0)));
INSERT Employee VALUES ('E2005005', 'Xiaodong Zhang', 'M', CAST(0x0000691C00000000 AS Datetime), 'Nancang 99#', '15607105243', CAST(0x0000839900000000 AS Datetime), 'Bussiness Section', 'Staff', CAST(1800 AS decimal(18, 0)));
INSERT Employee VALUES ('E2006001', 'Chenhui', 'M', CAST(0x00005DED00000000 AS Datetime), 'Nangchang 100#', '13607705352', CAST(0x000080BE00000000 AS Datetime), 'Office', 'Director', CAST(4000 AS decimal(18, 0)));
INSERT Employee VALUES ('E2006002', 'Hanmei', 'F', CAST(0x0000697F00000000 AS Datetime), 'Shanghai 6#', '13807805461', CAST(0x000081B300000000 AS Datetime), 'Bussiness Section', 'Staff', CAST(2600 AS decimal(18, 0)));
INSERT Employee VALUES ('E2006003', 'Liu Feng', 'F', CAST(0x000068B300000000 AS Datetime), 'Jiangxi 5#', '15907805578', CAST(0x0000820F00000000 AS Datetime), 'Bussiness Section', 'Staff', CAST(2500 AS decimal(18, 0)));
INSERT Employee VALUES ('E2007001', 'Fuping Wu', 'M', CAST(0x0000692500000000 AS Datetime), 'Nancang 12#', NULL, CAST(0x0000811A00000000 AS Datetime), 'Bussiness Section', 'Staff', CAST(2500 AS decimal(18, 0)));
INSERT Employee VALUES ('E2007002', 'Gao Daipeng', 'M', CAST(0x0000682800000000 AS Datetime), 'Nanchang 56#', NULL, CAST(0x0000832000000000 AS Datetime), 'Office', 'Secrectary', CAST(2000 AS decimal(18, 0)));
INSERT Employee VALUES ('E2008001', 'Shijie Chen', 'M', CAST(0x0000610900000000 AS Datetime), 'Jiangxi 12#', NULL, CAST(0x000081BB00000000 AS Datetime), 'Finance Section', 'Cashier', CAST(3200 AS decimal(18, 0)));
INSERT Employee VALUES ('E2008002', 'Zhang Liang', 'M', CAST(0x000066E700000000 AS Datetime), 'Shanghai 135#', NULL, CAST(0x0000837C00000000 AS Datetime), 'Bussiness Section', 'Staff', CAST(2700 AS decimal(18, 0)));
INSERT Employee VALUES ('E2008003', 'Meiying Huang', 'F', CAST(0x0000674000000000 AS Datetime), 'Shanghai 88#', NULL, CAST(0x0000820F00000000 AS Datetime), 'Bussiness Section', 'Staff', CAST(3100 AS decimal(18, 0)));
INSERT Employee VALUES ('E2008004', 'Hongbin Li', 'F', CAST(0x000067D700000000 AS Datetime), 'Nanchang 1#', NULL, CAST(0x000080FB00000000 AS Datetime), 'Bussiness Section', 'Staff', CAST(3400 AS decimal(18, 0)));
INSERT Employee VALUES ('E2008005', 'Xiaomei', 'F', CAST(0x0000651400000000 AS Datetime), 'Shenzhen 10#', NULL, CAST(0x000081A900000000 AS Datetime), 'Finance Section', 'Accountant', CAST(5000 AS decimal(18, 0)));

INSERT Product VALUES ('P20050001', '32M DRAM', 'Memory', CAST(80.70 AS decimal(7, 2)), 80);
INSERT Product VALUES ('P20050002', '17 Display', 'Display', CAST(700.00 AS decimal(7, 2)), 96);
INSERT Product VALUES ('P20050003', '120GB', 'Hard disk', CAST(300.00 AS decimal(7, 2)), 96);
INSERT Product VALUES ('P20050004', '3.5 Soft disk', 'Hardware', CAST(35.00 AS decimal(7, 2)), 100);
INSERT Product VALUES ('P20050005', 'Keyboard', 'Hardware', CAST(100.60 AS decimal(7, 2)), 97);
INSERT Product VALUES ('P20060001', 'VGA', 'Display', CAST(1200.60 AS decimal(7, 2)), 97);
INSERT Product VALUES ('P20060002', 'Net card', 'Hardware', CAST(66.00 AS decimal(7, 2)), 95);
INSERT Product VALUES ('P20060003', 'Pentium100CPU', 'Processor', CAST(200.00 AS decimal(7, 2)), 96);
INSERT Product VALUES ('P20070001', '1G DDR', 'Memory', CAST(256.00 AS decimal(7, 2)), 91);
INSERT Product VALUES ('P20070002', '52 CD disk', 'Hardware', CAST(200.00 AS decimal(7, 2)), 95);
INSERT Product VALUES ('P20070003', 'Dictionary', 'Book', CAST(100.00 AS decimal(7, 2)), 94);
INSERT Product VALUES ('P20070004', '9600bits/s Mordern', 'Hardware', CAST(320.00 AS decimal(7, 2)), 98);
INSERT Product VALUES ('P20080001', 'PentiumMotherboard', 'Motherboard', CAST(890.00 AS decimal(7, 2)), 93);
INSERT Product VALUES ('P20080002', 'SL-K8AMotherboard', 'Motherboard', CAST(1100.00 AS decimal(7, 2)), 93);
INSERT Product VALUES ('P20080003', '777FT Display', 'Display', CAST(900.00 AS decimal(7, 2)), 97);

INSERT OrderMaster VALUES ('200801090001', 'C20050001', 'E2005002', CAST(0x00009A1E00000000 AS Datetime), CAST(4600.00 AS decimal(9, 2)), 'I000000001');
INSERT OrderMaster VALUES ('200801090002', 'C20050004', 'E2005003', CAST(0x00009A1E00000000 AS Datetime), CAST(5746.00 AS decimal(9, 2)), 'I000000002');
INSERT OrderMaster VALUES ('200802190001', 'C20050001', 'E2005003', CAST(0x00009A4700000000 AS Datetime), CAST(5786.00 AS decimal(9, 2)), 'I000000004');
INSERT OrderMaster VALUES ('200802190002', 'C20070002', 'E2008002', CAST(0x00009A4700000000 AS Datetime), CAST(2270.00 AS decimal(9, 2)), 'I000000005');
INSERT OrderMaster VALUES ('200803010001', 'C20070002', 'E2008001', CAST(0x00009A5200000000 AS Datetime), CAST(1800.00 AS decimal(9, 2)), 'I000000006');
INSERT OrderMaster VALUES ('200803020001', 'C20050004', 'E2008003', CAST(0x00009A5300000000 AS Datetime), CAST(1000.00 AS decimal(9, 2)), 'I000000007');

INSERT OrderDetail VALUES ('200801090001', 'P20050001', 5, CAST(500.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200801090001', 'P20050002', 3, CAST(500.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200801090001', 'P20050003', 2, CAST(300.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200801090002', 'P20060002', 5, CAST(250.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200801090002', 'P20080001', 5, CAST(280.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200801090002', 'P20080002', 4, CAST(270.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200801090002', 'P20080003', 2, CAST(158.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200801090002', 'P20050001', 5, CAST(130.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200801090002', 'P20060001', 3, CAST(350.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200802190001', 'P20060003', 4, CAST(270.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200802190001', 'P20070001', 2, CAST(158.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200802190001', 'P20070002', 5, CAST(250.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200802190001', 'P20070003', 3, CAST(350.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200802190001', 'P20070004', 2, CAST(330.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200802190001', 'P20080001', 2, CAST(160.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200802190001', 'P20080002', 3, CAST(260.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200802190001', 'P20080003', 1, CAST(330.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200802190002', 'P20050003', 2, CAST(160.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200802190002', 'P20050005', 3, CAST(150.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200802190002', 'P20070001', 3, CAST(500.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200803010001', 'P20050001', 8, CAST(150.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200803010001', 'P20070001', 4, CAST(150.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200803020001', 'P20050001', 2, CAST(100.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200803020001', 'P20050002', 1, CAST(200.00 AS decimal(7, 2)));
INSERT OrderDetail VALUES ('200803020001', 'P20070003', 3, CAST(200.00 AS decimal(7, 2)));
