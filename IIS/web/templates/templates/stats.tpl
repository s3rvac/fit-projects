{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

{include file='module_header.tpl'}
	<h3>{$L_STANDARD_STATS}</h3>

	<br />

	{if $STANDARD_STATS}
		<div>
			<table class="main-table">
				<tr class="table-header">
					<td>{$L_STAT_NAME}</td>
					<td>{$L_STAT_VALUE}</td>
				</tr>
				{foreach item=value key=name from=$STANDARD_STATS}
					<tr>
						<td>{$name}</td>
						<td>{$value}</td>
					</tr>
				{/foreach}
			</table>
		</div>
	{else}
		<div>
			{$L_NO_STATS}
		</div>
	{/if}

	<br />

	{if $USER_GROUP eq $EXHIBITIONGROUND_MEMBER}
		<h3>{$L_EXTENDED_STATS}</h3>

		<br />

		{if $EXTENDED_STATS}
			<div>
				<table class="main-table">
					<tr class="table-header">
						<td>{$L_STAT_NAME}</td>
						<td>{$L_STAT_VALUE}</td>
					</tr>
					{foreach item=value key=name from=$EXTENDED_STATS}
						<tr>
							<td>{$name}</td>
							<td>{$value}</td>
						</tr>
					{/foreach}
				</table>
			</div>
		{else}
			<div>
				{$L_NO_STATS}
			</div>
		{/if}
	{/if}
{include file='module_footer.tpl'}
