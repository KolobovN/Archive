CREATE PROCEDURE dbo.SelectMonthWorkDays
	@FirstDay Date,
	@LastDay Date
AS
	SELECT * FROM dbo.WorkDays WHERE dbo.WorkDays.Date between @FirstDay and @LastDay
