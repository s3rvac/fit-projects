{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

{include file='module_header.tpl'}
	{if $ACTION eq 'view_pavilions'}
		{if $PAVILIONS_DATA}
			<form action="{$U_PAVILIONS_DELETE}" method="post">
				<table width="100%" border="0">
					<tr class="table-header">
						<td>
							<a href="{$U_ORDER_BY_NAME_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_PAVILION_NAME}
							<a href="{$U_ORDER_BY_NAME_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td>
							<a href="{$U_ORDER_BY_NUMBER_OF_FLOORS_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_NUMBER_OF_FLOORS}
							<a href="{$U_ORDER_BY_NUMBER_OF_FLOORS_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td class="view-small-column">{$L_DETAILS_PAVILION}</td>
						<td class="view-small-column">{$L_EDIT_PAVILION}</td>
						<td class="view-small-column">{$L_DELETE_PAVILION}</td>
					</tr>
					{foreach item=pavilion_data from=$PAVILIONS_DATA}
						<tr>
							<td>{$pavilion_data.name}</td>
							<td>{$pavilion_data.number_of_floors}</td>
							<td class="align-center"><a href="{$pavilion_data.details_pavilion_url}">{$L_DETAILS}</a></td>
							<td class="align-center"><a href="{$pavilion_data.edit_pavilion_url}">{$L_EDIT}</a></td>
							<td class="align-center"><input class="checkbox" type="checkbox" name="delete_pavilion[]" value="{$pavilion_data.pavilion_id}" /></td>
						</tr>
					{/foreach}
				</table>
				<div class="align-right">
					<input class="submit" type="submit" name="delete_pavilions" value="{$L_DELETE_SELECTED_PAVILIONS}" />
				</div>
			</form>
			{include file='module_pagination.tpl'}
		{else}
			<div class="align-center">
				{$L_NO_PAVILIONS_AVAILABLE}
			</div>
		{/if}
	{else}
		<form action="{$U_ACTION_PAVILION}" method="post">
			<table width="100%" border="0">
				<tr>
					<td class="table-label">{$L_PAVILION_NAME}:{if $ACTION neq 'details_pavilion'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_pavilion'}<input type="text" name="name" value="{$PAVILION_DATA.name}" />{else}{$PAVILION_DATA.name}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_NUMBER_OF_FLOORS}:{if $ACTION neq 'details_pavilion'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_pavilion'}<input type="text" name="number_of_floors" value="{$PAVILION_DATA.number_of_floors}" />{else}{$PAVILION_DATA.number_of_floors}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_NUMBER_OF_RESTAURANTS}:</td>
					<td class="table-input">{if $ACTION neq 'details_pavilion'}<input type="text" name="number_of_restaurants" value="{$PAVILION_DATA.number_of_restaurants}" />{else}{$PAVILION_DATA.number_of_restaurants}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_NUMBER_OF_BUFFETS}:</td>
					<td class="table-input">{if $ACTION neq 'details_pavilion'}<input type="text" name="number_of_buffets" value="{$PAVILION_DATA.number_of_buffets}" />{else}{$PAVILION_DATA.number_of_buffets}{/if}</td>
				</tr>
				<tr>
					<td class="table-label">{$L_NUMBER_OF_SOCIAL_SETTLEMENTS}:</td>
					<td class="table-input">{if $ACTION neq 'details_pavilion'}<input type="text" name="number_of_social_settlements" value="{$PAVILION_DATA.number_of_social_settlements}" />{else}{$PAVILION_DATA.number_of_social_settlements}{/if}</td>
				</tr>
			</table>
			<div class="action-buttons">
				{if $ACTION neq 'details_pavilion'}
					<input class="submit" type="submit" name="{$ACTION}" value="{$L_ACTION_PAVILION}" />
				{/if}
					<input class="submit" type="button" onclick="location.replace('{$U_BACK}');" value="{$L_BACK}" />
			</div>
			{if $ACTION neq 'details_pavilion'}
				<div class="align-left">
					<span class="mandatory-field">*</span> - {$L_MANDATORY_FIELDS}
				</div>
			{/if}
		</form>
	{/if}
{include file='module_footer.tpl'}
