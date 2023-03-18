interface PieceInitOpts {
	pos: { x: number; y: number } | undefined;
	taken: boolean;
	color: 'black' | 'white';
}

export type Piece = {
	name: string;
	value: number;

	pos: { x: number; y: number } | undefined;
	taken: boolean;
	color: 'black' | 'white';

	getLegalMoves(): { x: number; y: number }[];
	checkMove(): boolean;
	move(): boolean;
};

export class King implements Piece {
	name = 'king';
	value = 0;

	pos: { x: number; y: number } | undefined;
	taken: boolean;
	color: 'black' | 'white';

	constructor(c: PieceInitOpts) {
		this.pos = c.pos;
		this.taken = c.taken;
		this.color = c.color;
	}

	checkMove(): boolean {
		throw new Error('Method not implemented.');
	}

	move(): boolean {
		throw new Error('Method not implemented.');
	}

	getLegalMoves(): { x: number; y: number }[] {
		throw new Error('Method not implemented.');
	}
}

export class Queen implements Piece {
	name = 'queen';
	value = 9;

	pos: { x: number; y: number } | undefined;
	taken: boolean;
	color: 'black' | 'white';

	constructor(c: PieceInitOpts) {
		this.pos = c.pos;
		this.taken = c.taken;
		this.color = c.color;
	}

	checkMove(): boolean {
		throw new Error('Method not implemented.');
	}

	move(): boolean {
		throw new Error('Method not implemented.');
	}

	getLegalMoves(): { x: number; y: number }[] {
		throw new Error('Method not implemented.');
	}
}

export class Rook implements Piece {
	name = 'rook';
	value = 5;

	pos: { x: number; y: number } | undefined;
	taken: boolean;
	color: 'black' | 'white';

	constructor(c: PieceInitOpts) {
		this.pos = c.pos;
		this.taken = c.taken;
		this.color = c.color;
	}

	checkMove(): boolean {
		throw new Error('Method not implemented.');
	}

	move(): boolean {
		throw new Error('Method not implemented.');
	}

	getLegalMoves(): { x: number; y: number }[] {
		throw new Error('Method not implemented.');
	}
}

export class Bishop implements Piece {
	name = 'bishop';
	value = 3;

	pos: { x: number; y: number } | undefined;
	taken: boolean;
	color: 'black' | 'white';

	constructor(c: PieceInitOpts) {
		this.pos = c.pos;
		this.taken = c.taken;
		this.color = c.color;
	}

	checkMove(): boolean {
		throw new Error('Method not implemented.');
	}

	move(): boolean {
		throw new Error('Method not implemented.');
	}

	getLegalMoves(): { x: number; y: number }[] {
		throw new Error('Method not implemented.');
	}
}

export class Knight implements Piece {
	name = 'knight';
	value = 3;

	pos: { x: number; y: number } | undefined;
	taken: boolean;
	color: 'black' | 'white';

	constructor(c: PieceInitOpts) {
		this.pos = c.pos;
		this.taken = c.taken;
		this.color = c.color;
	}

	checkMove(): boolean {
		throw new Error('Method not implemented.');
	}

	move(): boolean {
		throw new Error('Method not implemented.');
	}

	getLegalMoves(): { x: number; y: number }[] {
		throw new Error('Method not implemented.');
	}
}

export class Pawn implements Piece {
	name = 'pawn';
	value = 1;

	pos: { x: number; y: number } | undefined;
	taken: boolean;
	color: 'black' | 'white';

	constructor(c: PieceInitOpts) {
		this.pos = c.pos;
		this.taken = c.taken;
		this.color = c.color;
	}

	checkMove(): boolean {
		throw new Error('Method not implemented.');
	}

	move(): boolean {
		throw new Error('Method not implemented.');
	}

	getLegalMoves(): { x: number; y: number }[] {
		throw new Error('Method not implemented.');
	}
}
