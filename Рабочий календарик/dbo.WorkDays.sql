CREATE TABLE [dbo].[WorkDays]
(
	[Date] DATE NOT NULL PRIMARY KEY, 
    [PSname] NVARCHAR(50) NULL, 
    [PSaddr] NVARCHAR(50) NULL, 
    [PSworktime] NVARCHAR(50) NULL, 
    [Cost] INT NULL, 
    [IsSubUrban] BIT NOT NULL
)
