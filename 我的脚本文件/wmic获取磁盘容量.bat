wmic logicaldisk where "DriveType=3 and DeviceID='D:'" get DeviceID,Size,FreeSpace
pause