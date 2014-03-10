{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

{include file='module_header.tpl'}
	{if $ACTION eq 'view_expositions'}
		{if $EXPOSITIONS_DATA}
			<form action="{$U_EXPOSITIONS_DELETE}" method="post">
				<table width="100%" border="0">
					<tr class="table-header">
						<td>
							<a href="{$U_ORDER_BY_CONTRACT_NUMBER_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_CONTRACT_N}
							<a href="{$U_ORDER_BY_CONTRACT_NUMBER_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td>
							<a href="{$U_ORDER_BY_EXHIBITION_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_EXHIBITION}
							<a href="{$U_ORDER_BY_EXHIBITION_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td>
							<a href="{$U_ORDER_BY_PLACE_ASCENDING}"><img src="{$TEMPLATES_PATH}images/ascending_order.gif" alt="{$L_ORDER_ASCENDING}"/></a>
							{$L_PLACE}
							<a href="{$U_ORDER_BY_PLACE_DESCENDING}"><img src="{$TEMPLATES_PATH}images/descending_order.gif" alt="{$L_ORDER_DESCENDING}"/></a>
						</td>
						<td class="view-small-column">{$L_DETAILS_EXPOSITION}</td>
						<td class="view-small-column">{$L_EDIT_EXPOSITION}</td>
						{if $USER_GROUP eq $EXHIBITIONGROUND_MEMBER}
							<td class="view-small-column">{$L_DELETE_EXPOSITION}</td>
						{/if}
					</tr>
					{foreach item=exposition_data from=$EXPOSITIONS_DATA}
						<tr>
							<td>{$exposition_data.contract_id}</td>
							<td>{$exposition_data.exhibition}</td>
							<td>
								{foreach item=place from=$PLACES}
									{if $exposition_data.place_id eq $place.place_id}{$place.pavilion} - {$place.number}{/if}
								{/foreach}
							</td>
							<td class="align-center"><a href="{$exposition_data.details_exposition_url}">{$L_DETAILS}</a></td>
							<td class="align-center"><a href="{$exposition_data.edit_exposition_url}">{$L_EDIT}</a></td>
							{if $USER_GROUP eq $EXHIBITIONGROUND_MEMBER}
								<td class="align-center"><input class="checkbox" type="checkbox" name="delete_exposition[]" value="{$exposition_data.exposition_id}" /></td>
							{/if}
						</tr>
					{/foreach}
				</table>
				{if $USER_GROUP eq $EXHIBITIONGROUND_MEMBER}
					<div class="align-right">
						<input class="submit" type="submit" name="delete_expositions" value="{$L_DELETE_SELECTED_EXPOSITIONS}" />
					</div>
				{/if}
			</form>
			{include file='module_pagination.tpl'}
		{else}
			<div class="align-center">
				{$L_NO_EXPOSITIONS_AVAILABLE}
			</div>
		{/if}
	{else}
		<form action="{$U_ACTION_EXPOSITION}" method="post">
			<table width="100%" border="0">
				{if $USER_GROUP eq $EXHIBITIONGROUND_MEMBER}
				<tr>
					<td class="table-label">{$L_CONTRACT_NUMBER}:{if $ACTION neq 'details_exposition'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">
						{if $ACTION neq 'details_exposition'}
							{if $USER_GROUP eq $EXHIBITIONGROUND_MEMBER}
								<select name="contract_id">
									{foreach item=contract from=$CONTRACTS}
										<option value="{$contract.contract_id}" {if $EXPOSITION_DATA.contract_id eq $contract.contract_id} selected="selected"{/if}>{$contract.contract_id}</option>
									{/foreach}
								</select>
							{else}
								{foreach item=contract from=$CONTRACTS}
									{if $EXPOSITION_DATA.contract_id eq $contract.contract_id}{$contract.contract_id}{/if}
								{/foreach}
							{/if}
						{else}
							<a href="{$EXPOSITION_DATA.details_contract_url}">{$EXPOSITION_DATA.contract_id}</a>
						{/if}
					</td>
				</tr>
				{/if}
				<tr>
					<td class="table-label">{$L_EXHIBITION}:{if $ACTION neq 'details_exposition'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">
						{if $ACTION neq 'details_exposition'}
							{if $USER_GROUP eq $EXHIBITIONGROUND_MEMBER}
								<select name="exhibition_id">
									{foreach item=exhibition from=$EXHIBITIONS}
										<option value="{$exhibition.exhibition_id}" {if $EXPOSITION_DATA.exhibition_id eq $exhibition.exhibition_id} selected="selected"{/if}>{$exhibition.name}</option>
									{/foreach}
								</select>
							{else}
								{foreach item=exhibition from=$EXHIBITIONS}
									{if $EXPOSITION_DATA.exhibition_id eq $exhibition.exhibition_id}{$exhibition.name}{/if}
								{/foreach}
							{/if}
						{else}
							{foreach item=exhibition from=$EXHIBITIONS}
								{if $EXPOSITION_DATA.exhibition_id eq $exhibition.exhibition_id}<a href="{$EXPOSITION_DATA.details_exhibition_url}">{$exhibition.name}</a>{/if}
							{/foreach}
						{/if}
					</td>
				</tr>
				<tr>
					<td class="table-label">{$L_PLACE}:{if $ACTION neq 'details_exposition'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">
						{if $ACTION neq 'details_exposition'}
							{if $USER_GROUP eq $EXHIBITIONGROUND_MEMBER}
								<select name="place_id">
									{foreach item=place from=$PLACES}
										<option value="{$place.place_id}" {if $EXPOSITION_DATA.place_id eq $place.place_id} selected="selected"{/if}>{$place.pavilion} - {$place.number}</option>
									{/foreach}
								</select>
							{else}
								{foreach item=place from=$PLACES}
									{if $EXPOSITION_DATA.place_id eq $place.place_id}{$place.pavilion} - {$place.number}{/if}
								{/foreach}
							{/if}
						{else}
							{foreach item=place from=$PLACES}
								{if $EXPOSITION_DATA.place_id eq $place.place_id}
									{if $USER_GROUP eq $EXHIBITIONGROUND_MEMBER}
										<a href="{$EXPOSITION_DATA.details_place_url}">{$place.pavilion} - {$place.number}</a>
									{else}
										<a href="{$EXPOSITION_DATA.details_pavilion_url}">{$place.pavilion} - {$place.number}</a>
									{/if}
								{/if}
							{/foreach}
						{/if}
					</td>
				</tr>
				{if ($USER_GROUP eq $EXHIBITIONGROUND_MEMBER) and ($ACTION eq 'details_exposition')}
					<tr>
						<td class="table-label">{$L_EXPOSITION_PRICE}:</td>
						<td class="table-input">{$EXPOSITION_PRICE},-</td>
					</tr>
				{/if}
				<tr>
					<td class="table-label">{$L_BRANCH}:{if $ACTION neq 'details_exposition'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">
						{if $ACTION neq 'details_exposition'}
							<select name="branch_id">
								{foreach item=branch from=$BRANCHES}
									<option value="{$branch.branch_id}" {if $EXPOSITION_DATA.branch_id eq $branch.branch_id} selected="selected"{/if}>{$branch.name}</option>
								{/foreach}
							</select>
						{else}
							{foreach item=branch from=$BRANCHES}
								{if $EXPOSITION_DATA.branch_id eq $branch.branch_id}<a href="{$EXPOSITION_DATA.details_branch_url}">{$branch.name}</a>{/if}
							{/foreach}
						{/if}
					</td>
				</tr>
				<tr>
					<td class="table-label">{$L_DESCRIPTION}:{if $ACTION neq 'details_exposition'}<span class="mandatory-field">*</span>{/if}</td>
					<td class="table-input">{if $ACTION neq 'details_exposition'}<textarea name="description" rows="5" cols="40">{$EXPOSITION_DATA.description}</textarea>{else}{$EXPOSITION_DATA.description}{/if}</td>
				</tr>
			</table>
			<div class="action-buttons">
				{if $ACTION neq 'details_exposition'}
					<input class="submit" type="submit" name="{$ACTION}" value="{$L_ACTION_EXPOSITION}" />
				{/if}
					<input class="submit" type="button" onclick="location.replace('{$U_BACK}');" value="{$L_BACK}" />
			</div>
			{if $ACTION neq 'details_exposition'}
				<div class="align-left">
					<span class="mandatory-field">*</span> - {$L_MANDATORY_FIELDS}
				</div>
			{/if}
		</form>
	{/if}
{include file='module_footer.tpl'}
