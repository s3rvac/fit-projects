{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

{include file='module_header.tpl'}
	<h3>{$L_FORTHCOMING_EXHIBITION}</h3>

	<br />

	{if $FORTHCOMING_EXHIBITION}
		<div>
			<table class="main-table">
				<tr class="table-header">
					<td>{$L_EXHIBITION_NAME}</td>
					<td>{$L_EXHIBITION_START}</td>
					<td>{$L_EXHIBITION_END}</td>
				</tr>
				<tr>
					<td><a href="{$FORTHCOMING_EXHIBITION.details_exhibition_url}">{$FORTHCOMING_EXHIBITION.name}</a></td>
					<td>{$FORTHCOMING_EXHIBITION.start}</td>
					<td>{$FORTHCOMING_EXHIBITION.end}</td>
				</tr>
			</table>
		</div>

		<br />

		<h3>{$L_EXPOSITIONS_ON_FORTHCOMING_EXHIBITION}</h3>

		<br />

		{if $EXPOSITIONS}
			<div>
				<table class="main-table">
					<tr class="table-header">
						<td>{$L_DETAILS}</td>
						<td>{$L_COMPANY}</td>
						<td>{$L_BRANCH}</td>
						<td>{$L_DESCRIPTION}</td>
					</tr>
					{foreach item=exposition from=$EXPOSITIONS}
						<tr>
							<td><a href="{$exposition.details_exposition_url}">{$L_DETAILS}</a></td>
							<td><a href="{$exposition.details_company_url}">{$exposition.company}</a></td>
							<td>{$exposition.branch}</td>
							<td>{$exposition.description}</td>
						</tr>
					{/foreach}
				</table>
			</div>
		{else}
			<div>
				{$L_NO_EXPOSITIONS}
			</div>
		{/if}
	{else}
		<div>
			{$L_NO_FORTHCOMING_EXHIBITION}
		</div>
	{/if}
{include file='module_footer.tpl'}
