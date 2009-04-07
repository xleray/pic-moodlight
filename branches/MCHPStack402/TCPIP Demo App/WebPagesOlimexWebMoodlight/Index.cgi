<meta http-equiv="refresh" content="5">
<html>
	<body>
		<b>Microchip TCP/IP Stack</b><br>
		Stack version: %16<br>
		Build date: %17<br>
		<br>
		<form method="get" action="0">
			<b>Actions</b><br>
			Toggle RELAYs:<br>
			<input type="submit" name="1" value="RELAY2"></input>
			<input type="submit" name="0" value="RELAY1"></input>
		</form>
		<form method="get" action="1">
			Write to LCD:
			<input type="text" name="3" size="16"></input>
			<input type="submit" value="Write"></input>
		</form><br>
		<form method="get" action="3">
			Write to LEDS0:
			<input type="text" name="0" size="16"></input>
			<input type="submit" value="Write"></input>
		</form><br>
		<form method="get" action="3">
			Write to LEDS1:
			<input type="text" name="1" size="16"></input>
			<input type="submit" value="Write"></input>
		</form><br>
		<form method="get" action="3">
			Write to LEDS2:
			<input type="text" name="2" size="16"></input>
			<input type="submit" value="Write"></input>
		</form><br>
		<form method="get" action="3">
			Write to LEDS3:
			<input type="text" name="3" size="16"></input>
			<input type="submit" value="Write"></input>
		</form><br>
		<b>Status</b><br>
		Pot0: %02<br>
		Temp: %03<br>
		Button1: %04<br>
		Button2: %0E<br>
		LED Board:<br>
		<table cellpadding="3">
			<tr>
				<td>%20</td>
				<td>%21</td>
			</tr>
			<tr>
				<td>%22</td>
				<td>%23</td>
			</tr>
		</table><br>
	</body>
</html>