package scut.hujie.visitorPattern;

public interface ComputerPart {
	public void accept(ComputerPartVisitor computerPartVisitor);
}
