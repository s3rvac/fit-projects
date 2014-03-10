{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

{include file='module_header.tpl'}
	{if $ACTION eq 'view_places'}
		{if $PLACES_DATA}
			<form action="{$U_PLACES_DELETE}" method="post">
				<table width="100%" border="0">
					<tr class="table-header">
						<td>
							<a href="{$U_ORDER_BY_NUMBER_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_PLACE_NUMBER}
							<a href="{$U_ORDER_BY_NUMBER_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td>
							<a href="{$U_ORDER_BY_PAVILION_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_PAVILION}
							<a href="{$U_ORDER_BY_PAVILION_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td>
							<a href="{$U_ORDER_BY_PRICE_CATEGORY_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_PRICE_CATEGORY}
							<a href="{$U_ORDER_BY_PRICE_CATEGORY_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td class="view-small-column">{$L_DETAILS_PLACE}</td>
						<td class="view-small-column">{$L_EDIT_PLACE}</td>
						<td class="view-small-column">{$L_DELETE_PLACE}</td>
					</tr>
					{foreach item=place_data from=$PLACES_DATA}
						<tr>
							<td>{$place_data.number}</td>
							<td>{$place_data.pavilion}</td>
							<td>{$place_data.price_category}</td>
							<td class="align-center"><a href="{$place_data.details_place_url}">{$L_DETAILS}</a></td>
							<td class="align-center"><a href="{$place_data.edit_place_url}">{$L_EDIT}</a></td>
							<td class="align-center"><input class="checkbox" type="checkbox" name="delete_place[]" value="{$place_data.place_id}" /></td>
						</tr>
					{/foreach}
				</table>
				<div class="align-right">
					<input class="submit" type="submit" name="delete_places" value="{$L_DELETE_SELECTED_PLACES}" />
				</div>
			</form>
			{include file='module_pagination.tpl'}
		{else}
			<div class="align-center">
				{$L_NO_PLACES_AVAILABLE}
			</div>
		{/if}
	{else}
		<form action="{$U_ACTION_PLACE}" method="post">
			<table width="100%" border="0">
				<tr>
					<td class="table-label">{$L_PAVILION}:{if $ACTION neq 'details_place'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">
						{if $ACTION neq 'details_place'}
							<select name="pavilion_id">
								{foreach item=name key=pavilion_id from=$PAVILIONS}
									<option value="{$pavilion_id}" {if $PLACE_DATA.pavilion_id eq $pavilion_id} selected="selected"{/if}>{$name}</option>
								{/foreach}
							</select>
						{else}
							{foreach item=name key=pavilion_id from=$PAVILIONS}
								{if $PLACE_DATA.pavilion_id eq $pavilion_id}{$name}{/if}
							{/foreach}
						{/if}
					</td>
				</tr>
				<tr>
					<td class="table-label">{$L_PLACE_NUMBER}:{if $ACTION neq 'details_place'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_place'}<input type="text" name="number" value="{$PLACE_DATA.number}" />{else}{$PLACE_DATA.number}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_AREA|smarty:nodefaults}:{if $ACTION neq 'details_place'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_place'}<input type="text" name="area" value="{$PLACE_DATA.area}" />{else}{$PLACE_DATA.area}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_PRICE_CATEGORY}:{if $ACTION neq 'details_place'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">
						{if $ACTION neq 'details_place'}
							<select name="price_category_id">
								{foreach item=name key=price_category_id from=$PRICE_CATEGORIES}
									<option value="{$price_category_id}" {if $PLACE_DATA.price_category_id eq $price_category_id} selected="selected"{/if}>{$name}</option>
								{/foreach}
							</select>
						{else}
							{foreach item=name key=price_category_id from=$PRICE_CATEGORIES}
								{if $PLACE_DATA.price_category_id eq $price_category_id}<a href="{$PLACE_DATA.details_price_category_url}">{$name}</a>{/if}
							{/foreach}
						{/if}
					</td>
				</tr>
			</table>
			<div class="action-buttons">
				{if $ACTION neq 'details_place'}
					<input class="submit" type="submit" name="{$ACTION}" value="{$L_ACTION_PLACE}" />
				{/if}
					<input class="submit" type="button" onclick="location.replace('{$U_BACK}');" value="{$L_BACK}" />
			</div>
			{if $ACTION neq 'details_place'}
				<div class="align-left">
					<span class="mandatory-field">*</span> - {$L_MANDATORY_FIELDS}
				</div>
			{/if}
		</form>
	{/if}
{include file='module_footer.tpl'}
