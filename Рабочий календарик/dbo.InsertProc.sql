CREATE PROCEDURE [dbo].[InsertWorkDayData]
	@Date date,
	@PSname nvarchar(50),
	@PSaddr nvarchar(50),
	@PSworktime nvarchar(50),
	@cost int,
	@IsSubUrban bit
AS
	INSERT INTO WorkDays(Date, PSname, PSaddr, PSworktime, Cost, IsSubUrban)
	VALUES (@Date, @PSname, @PSaddr, @PSworktime, @cost, @IsSubUrban)
GO
