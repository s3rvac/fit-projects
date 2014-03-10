{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

{include file='module_header.tpl'}
	{if $DEBTORS_DATA}
		<form action="{$U_WARN_DEBTORS}" method="post">
			<table width="100%" border="0">
				<tr class="table-header">
					<td>
						<a href="{$U_ORDER_BY_COMPANY_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
						{$L_COMPANY}
						<a href="{$U_ORDER_BY_COMPANY_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
					</td>
					<td>
						<a href="{$U_ORDER_BY_TOTAL_PRICE_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
						{$L_TOTAL_PRICE}
						<a href="{$U_ORDER_BY_TOTAL_PRICE_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
					</td>
					<td>
						<a href="{$U_ORDER_BY_SETTLED_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
						{$L_SETTLED}
						<a href="{$U_ORDER_BY_SETTLED_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
					</td>
					<td>
						<a href="{$U_ORDER_BY_OWED_PRICE_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
						{$L_OWED_PRICE}
						<a href="{$U_ORDER_BY_OWED_PRICE_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
					</td>
					<td>{$L_WARN_DEBTOR}</td>
				</tr>
				{foreach item=debtor_data from=$DEBTORS_DATA}
					<tr>
						<td><a href="{$debtor_data.details_company_url}">{$debtor_data.company}</a></td>
						<td>{$debtor_data.total_price},-</td>
						<td>{$debtor_data.settled},-</td>
						<td>{$debtor_data.owed_price},-</td>
						<td class="align-center"><input class="checkbox" type="checkbox" name="debtors[]" value="{$debtor_data.company_id}" /></td>
					</tr>
				{/foreach}
			</table>
			<div class="align-right">
				<input class="submit" type="submit" name="warn_all_debtors" value="{$L_WARN_ALL_DEBTORS}" />
				&nbsp;
				<input class="submit" type="submit" name="warn_debtors" value="{$L_WARN_SELECTED_DEBTORS}" />

			</div>
		</form>
		{include file='module_pagination.tpl'}
	{else}
		<div class="align-center">
			{$L_NO_DEBTORS_AVAILABLE}
		</div>
	{/if}
{include file='module_footer.tpl'}
