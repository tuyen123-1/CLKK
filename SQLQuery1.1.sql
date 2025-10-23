-- =============================
-- TẠO DATABASE VÀ SỬ DỤNG
-- =============================
CREATE DATABASE Quanlycoffe;
GO

USE Quanlycoffe;
GO

-- =============================
-- BẢNG BÀN (TableFood)
-- =============================
CREATE TABLE dbo.TableFood
(
    id INT IDENTITY PRIMARY KEY,
    name NVARCHAR(100) NOT NULL DEFAULT N'chưa đặt tên',
    status NVARCHAR(100) NOT NULL DEFAULT N'trống' -- trống || có người 
);
GO

-- =============================
-- BẢNG TÀI KHOẢN (Account)
-- =============================
CREATE TABLE dbo.Account
(
    displayname NVARCHAR(100) NOT NULL DEFAULT N'tuyencoi',
    username NVARCHAR(100) PRIMARY KEY,
    password NVARCHAR(100) NOT NULL DEFAULT 0,
    type INT NOT NULL DEFAULT 0 --1: admin && 0: staff
);
GO

-- =============================
-- BẢNG LOẠI MÓN (FoodCategory)
-- =============================
CREATE TABLE dbo.FoodCategory
(
    id INT IDENTITY PRIMARY KEY,
    name NVARCHAR(100) NOT NULL DEFAULT N'chưa đặt tên'
);
GO

-- =============================
-- BẢNG MÓN (Food)
-- =============================
CREATE TABLE dbo.Food
(
    id INT IDENTITY PRIMARY KEY,
    name NVARCHAR(100) NOT NULL DEFAULT N'chưa đặt tên', 
    idcategory INT NOT NULL, -- loại nào
    price FLOAT NOT NULL DEFAULT 0, -- giá
    FOREIGN KEY (idcategory) REFERENCES dbo.FoodCategory(id)
);
GO

-- =============================
-- BẢNG HÓA ĐƠN (Bill)
-- =============================
CREATE TABLE dbo.Bill
(
    id INT IDENTITY PRIMARY KEY, 
    datecheckin DATETIME NOT NULL DEFAULT GETDATE(), 
    datecheckout DATETIME,
    idtable INT NOT NULL,
    status INT NOT NULL DEFAULT 0, -- 1: đã thanh toán, 0: chưa thanh toán
    FOREIGN KEY (idtable) REFERENCES dbo.TableFood(id)
);
GO

-- =============================
-- BẢNG CHI TIẾT HÓA ĐƠN (BillInfo)
-- =============================
CREATE TABLE dbo.BillInfo
(
    id INT IDENTITY PRIMARY KEY, 
    idbill INT NOT NULL,
    idfood INT NOT NULL, 
    count INT NOT NULL DEFAULT 0,
    FOREIGN KEY (idbill) REFERENCES dbo.Bill(id),
    FOREIGN KEY (idfood) REFERENCES dbo.Food(id)
);
GO

-- =============================
-- THÊM DỮ LIỆU MẪU
-- =============================
INSERT INTO dbo.Account (username, displayname, password, type)
VALUES (N'k9', N'rongk9', N'1', 1);

INSERT INTO dbo.Account (username, displayname, password, type)
VALUES (N'staff', N'staff', N'1', 0);
GO

-- =============================
-- KIỂM TRA KẾT QUẢ
-- =============================
SELECT * FROM dbo.Account;
GO
