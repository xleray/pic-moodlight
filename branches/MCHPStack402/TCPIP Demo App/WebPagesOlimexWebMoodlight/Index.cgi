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
			Write to LEDS:
			<input type="text" name="0" size="16"></input>
			<input type="submit" value="Write"></input>
		</form><br>
		<b>Status</b><br>
		AN0: %02<br>
		AN1: %03<br>
		Temp: %18<br>
		Button1: %04<br>
		LED Board:<br>
		<table cellpadding="3">
			<tr>
				<td>
					<table cellpadding="3">
						<tr>
							<td>%40</td>
							<td>%41</td>
						</tr>
						<tr>
							<td>%42</td>
							<td>%43</td>
						</tr>
					</table>
				</td>
				<td>
					<table cellpadding="3">
						<tr>
							<td>%50</td>
							<td>%51</td>
						</tr>
						<tr>
							<td>%52</td>
							<td>%53</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr>
				<td>
					<table cellpadding="3">
						<tr>
							<td>%60</td>
							<td>%61</td>
						</tr>
						<tr>
							<td>%62</td>
							<td>%63</td>
						</tr>
					</table>
				</td>
				<td>
					<table cellpadding="3">
						<tr>
							<td>%70</td>
							<td>%71</td>
						</tr>
						<tr>
							<td>%72</td>
							<td>%73</td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
		<BR>
		<table cellpadding="3">
			<tr>
				<td>
					<table cellpadding="3">
						<tr>
							<td bgcolor="#%40">&nbsp;</td>
							<td bgcolor="#%41">&nbsp;</td>
						</tr>
						<tr>
							<td bgcolor="#%42">&nbsp;</td>
							<td bgcolor="#%43">&nbsp;</td>
						</tr>
					</table>
				</td>
				<td>
					<table cellpadding="3">
						<tr>
							<td bgcolor="#%50">&nbsp;</td>
							<td bgcolor="#%51">&nbsp;</td>
						</tr>
						<tr>
							<td bgcolor="#%52">&nbsp;</td>
							<td bgcolor="#%53">&nbsp;</td>
						</tr>
					</table>
				</td>
			</tr>
			<tr>
				<td>
					<table cellpadding="3">
						<tr>
							<td bgcolor="#%60">&nbsp;</td>
							<td bgcolor="#%61">&nbsp;</td>
						</tr>
						<tr>
							<td bgcolor="#%62">&nbsp;</td>
							<td bgcolor="#%63">&nbsp;</td>
						</tr>
					</table>
				</td>
				<td>
					<table cellpadding="3">
						<tr>
							<td bgcolor="#%70">&nbsp;</td>
							<td bgcolor="#%71">&nbsp;</td>
						</tr>
						<tr>
							<td bgcolor="#%72">&nbsp;</td>
							<td bgcolor="#%73">&nbsp;</td>
						</tr>
					</table>
				</td>
			</tr>
		</table><br>
	</body>
</html>