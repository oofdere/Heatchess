import type { Piece } from './pieces';

type P = Piece;

export class Board {
	flatBoard: Array<P>;
	matrixBoard: Array<Array<P>>;

	constructor() {
		this.flatBoard = new Array<P>(64);
		this.matrixBoard = [
			this.flatBoard.slice(0, 7),
			this.flatBoard.slice(0 + 8, 7 + 8),
			this.flatBoard.slice(0 + 8 * 2, 7 + 8 * 2),
			this.flatBoard.slice(0 + 8 * 3, 7 + 8 * 3),
			this.flatBoard.slice(0 + 8 * 4, 7 + 8 * 4),
			this.flatBoard.slice(0 + 8 * 5, 7 + 8 * 5),
			this.flatBoard.slice(0 + 8 * 6, 7 + 8 * 6),
			this.flatBoard.slice(0 + 8 * 7, 7 + 8 * 7)
		];

		console.log(this.flatBoard);
	}
}
