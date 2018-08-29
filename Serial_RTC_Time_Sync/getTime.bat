:UnixTime  [ReturnVar]  [TimeStamp]
::
:: Computes the Unix time from the current local time as reported by the
:: operating system. The Unix time is the number of seconds that have elapsed
:: since midnight Coordinated Universal Time (UTC), January 1, 1970, not
:: counting leap seconds.
::
:: The result is returned in variable ReturnVar,
:: or the result is echoed if ReturnVar is not specified
::
:: If the TimeStamp is provided in the 2nd parameter, then the Unix time for
:: the TimeStamp is computed, rather then for the current time.
::
:: The TimeStamp must have the same format as used by WMIC:
::
::   YYYYMMDDhhmmss.ffffffSzzz
::
:: where:
::
::   YYYY   = gregorian year
::   MM     = month
::   DD     = day
::   hh     = hour in 24 hour format
::   mm     = minute
::   ss     = seconds
::   ffffff = fractional seconds (microseconds)
::   S      = timezone sign: + or -
::   zzz    = timezone: minutes difference from GMT
::
:: Each component must be zero prefixed as needed to maintain the proper width.
::
:: The ReturnVar parameter must be provided in order to use the TimeStamp.
:: A ReturnVar of "" will function the same as no ReturnVar. This enables the
:: specification of a TimeStamp without an actual ReturnVar.
::
@echo off
setlocal
set "ts=%~2"
if not defined ts for /f "skip=1 delims=" %%A in ('wmic os get localdatetime') do if not defined ts set "ts=%%A"
set /a "yy=10000%ts:~0,4% %% 10000, mm=100%ts:~4,2% %% 100, dd=100%ts:~6,2% %% 100"
set /a "dd=dd-2472663+1461*(yy+4800+(mm-14)/12)/4+367*(mm-2-(mm-14)/12*12)/12-3*((yy+4900+(mm-14)/12)/100)/4"
set /a ss=(((1%ts:~8,2%*60)+1%ts:~10,2%)*60)+1%ts:~12,2%-366100-%ts:~21,1%((1%ts:~22,3%*60)-60000)
set /a ss+=dd*86400
endlocal & if "%~1" neq "" (set %~1=%ss%) else echo %ss%
exit /b