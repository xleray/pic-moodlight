<meta http-equiv="refresh" content="5">
<html>
	<body>
		<b>Microchip TCP/IP Stack</b><br>
		Stack version: %16<br>
		Build date: %17<br>
		<P>
		<b>Actions</b><br>
		<form method="get" action="0">
			Toggle Onboad LED:
			<input type="submit" name="0" value="LED0"></input>
		</form>
		<form method="get" action="3">
			Write to LEDS:
			<input type="text" name="0" size="16"></input>
			<input type="submit" value="Submit"></input>
		</form>
		</P>
		<P>
		<b>Example strings:</b>:<br>
			All Red 10900900900900FF<br>
			All Red 2F00FF<br>
			All Green 10090090090090FF<br>
			All Green 20F0FF<br>
			All Blue 10009009009009FF<br>
			All Blue 200FFF<br>
			Red Green Blue White 2F000120F002200F032FFF04<br>
			Rainbow 10F00A50A00A5501105A00F05A50A50210500555000005031055A05A00A00F04
		</P>
		<P>
		<b>Status</b>:<br>
		AN0: %02<br>
		AN1: %03<br>
		Temp: %18<br>
		Button1: %04<br>
		</P>
		<P>
		<b>LED Board:</b><br>
		<table cellpadding="0">
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
		<BR><b>LED Board Representation:</b><BR>
		<table cellpadding="0" bgcolor="#000000">
			<tr>
				<td>
					<table cellpadding="0">
						<tr>
							<td bgcolor="#%40"><img src="LEDb.png" width="25" height="25"></img></td>
							<td bgcolor="#%41"><img src="LEDb.png" width="25" height="25"></img></td>
						</tr>
						<tr>
							<td bgcolor="#%42"><img src="LEDb.png" width="25" height="25"></img></td>
							<td bgcolor="#%43"><img src="LEDb.png" width="25" height="25"></img></td>
						</tr>
					</table>
				</td>
				<td>
					<table cellpadding="0">
						<tr>
							<td bgcolor="#%50"><img src="LEDb.png" width="25" height="25"></img></td>
							<td bgcolor="#%51"><img src="LEDb.png" width="25" height="25"></img></td>
						</tr>
						<tr>
							<td bgcolor="#%52"><img src="LEDb.png" width="25" height="25"></img></td>
							<td bgcolor="#%53"><img src="LEDb.png" width="25" height="25"></img></td>
						</tr>
					</table>
				</td>
			</tr>
			<tr>
				<td>
					<table cellpadding="0">
						<tr>
							<td bgcolor="#%60"><img src="LEDb.png" width="25" height="25"></img></td>
							<td bgcolor="#%61"><img src="LEDb.png" width="25" height="25"></img></td>
						</tr>
						<tr>
							<td bgcolor="#%62"><img src="LEDb.png" width="25" height="25"></img></td>
							<td bgcolor="#%63"><img src="LEDb.png" width="25" height="25"></img></td>
						</tr>
					</table>
				</td>
				<td>
					<table cellpadding="0">
						<tr>
							<td bgcolor="#%70"><img src="LEDb.png" width="25" height="25"></img></td>
							<td bgcolor="#%71"><img src="LEDb.png" width="25" height="25"></img></td>
						</tr>
						<tr>
							<td bgcolor="#%72"><img src="LEDb.png" width="25" height="25"></img></td>
							<td bgcolor="#%73"><img src="LEDb.png" width="25" height="25"></img></td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
		</P>
	</body>
</html>