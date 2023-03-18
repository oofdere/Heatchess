<script lang="ts">
	import { Board } from '$lib/board';

	const board = new Board();
</script>

<table />
{#each board.matrixBoard as row}
	<tr>
		{#each row as p}
			<td>{p}</td>
		{/each}
	</tr>
{/each}
