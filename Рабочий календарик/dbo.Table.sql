CREATE TABLE [dbo].[Table]
(
	[Date] DATE NOT NULL PRIMARY KEY, 
    [PSname] NVARCHAR(MAX) NULL, 
    [PSaddr] NVARCHAR(MAX) NULL, 
    [PSworktime] NVARCHAR(MAX) NULL, 
    [Cost] INT NULL, 
    [IsSubUrban] BIT NOT NULL
)
