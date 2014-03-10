{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

{include file='module_header.tpl'}
	<br />

	<form action="{$U_SEARCH}" method="get">
		<div>
			<strong>{$L_SEARCH_QUERY}: </strong>
			<input type="text" name="query" size="24" value="{$QUERY}" />
			<input type="hidden" name="module" value="search" />
			<input class="submit" type="submit" value="{$L_SEARCH}" />
		</div>
	</form>

	<br />

	{if $SEARCH_DONE}
		<h3>{$L_SEARCH_RESULTS}</h3>

		<br />

		<div>
			{if $SEARCH_RESULTS and $RESULTS_FOUND}
				{foreach item=category from=$SEARCH_RESULTS}
					{if $category.results.data[0]}
						<strong>{$category.name}:</strong>
						<table class="main-table">
							<tr class="table-header">
								{foreach item=column_name from=$category.results.columns}
									<td>{$column_name}</td>
								{/foreach}
							</tr>
							{foreach item=data from=$category.results.data}
								<tr>
									{foreach item=item from=$data}
										<td>{if $item.url}<a href="{$item.url}">{/if}{$item.value}{if $item.url}</a>{/if}</td>
									{/foreach}
								</tr>
							{/foreach}
						</table>
						<br />
					{/if}
				{/foreach}
			{else}
				{$L_NO_SEARCH_RESULTS}
			{/if}
		</div>
	{/if}
{include file='module_footer.tpl'}
