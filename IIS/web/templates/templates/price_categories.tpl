{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

{include file='module_header.tpl'}
	{if $ACTION eq 'view_price_categories'}
		{if $PRICE_CATEGORIES_DATA}
			<form action="{$U_PRICE_CATEGORIES_DELETE}" method="post">
				<table width="100%" border="0">
					<tr class="table-header">
						<td>
							<a href="{$U_ORDER_BY_NAME_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_PRICE_CATEGORY_NAME}
							<a href="{$U_ORDER_BY_NAME_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td>
							<a href="{$U_ORDER_BY_PRICE_FOR_M2_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_PRICE_FOR_M2|smarty:nodefaults}
							<a href="{$U_ORDER_BY_PRICE_FOR_M2_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td class="view-small-column">{$L_DETAILS_PRICE_CATEGORY}</td>
						<td class="view-small-column">{$L_EDIT_PRICE_CATEGORY}</td>
						<td class="view-small-column">{$L_DELETE_PRICE_CATEGORY}</td>
					</tr>
					{foreach item=price_category_data from=$PRICE_CATEGORIES_DATA}
						<tr>
							<td>{$price_category_data.name}</td>
							<td>{$price_category_data.price_for_m2},-</td>
							<td class="view-small-column"><a href="{$price_category_data.details_price_category_url}">{$L_DETAILS}</a></td>
							<td class="view-small-column"><a href="{$price_category_data.edit_price_category_url}">{$L_EDIT}</a></td>
							<td class="view-small-column"><input class="checkbox" type="checkbox" name="delete_price_category[]" value="{$price_category_data.price_category_id}" /></td>
						</tr>
					{/foreach}
				</table>
				<div class="align-right">
					<input class="submit" type="submit" name="delete_price_categories" value="{$L_DELETE_SELECTED_PRICE_CATEGORIES}" />
				</div>
			</form>
			{include file='module_pagination.tpl'}
		{else}
			<div class="align-center">
				{$L_NO_PRICE_CATEGORIES_AVAILABLE}
			</div>
		{/if}
	{else}
		<form action="{$U_ACTION_PRICE_CATEGORY}" method="post">
			<table width="100%" border="0">
				<tr>
					<td class="table-label">{$L_PRICE_CATEGORY_NAME}:{if $ACTION neq 'details_price_category'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_price_category'}<input type="text" name="name" value="{$PRICE_CATEGORY_DATA.name}" />{else}{$PRICE_CATEGORY_DATA.name}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_PRICE_FOR_M2|smarty:nodefaults}:{if $ACTION neq 'details_price_category'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_price_category'}<input type="text" name="price_for_m2" value="{$PRICE_CATEGORY_DATA.price_for_m2}" />{else}{$PRICE_CATEGORY_DATA.price_for_m2},-{/if}</td>
				</tr>
			</table>
			<div class="align-center">
				{if $ACTION neq 'details_price_category'}
					<input class="submit" type="submit" name="{$ACTION}" value="{$L_ACTION_PRICE_CATEGORY}" />
				{/if}
					<input class="submit" type="button" onclick="location.replace('{$U_BACK}');" value="{$L_BACK}" />
			</div>
			{if $ACTION neq 'details_price_category'}
				<div class="align-left">
					<span class="mandatory-field">*</span> - {$L_MANDATORY_FIELDS}
				</div>
			{/if}
		</form>
	{/if}
{include file='module_footer.tpl'}
