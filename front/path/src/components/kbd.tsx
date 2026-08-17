import React from "react";

interface KbdProps {
  /** The key label, e.g. "K", "⌘", "Enter", "Esc" */
  children: React.ReactNode;
  /** Visual size */
  size?: "sm" | "md" | "lg";
}

const sizeStyles = {
  sm: { minWidth: "20px", height: "20px", fontSize: "11px", padding: "0 5px" },
  md: { minWidth: "26px", height: "26px", fontSize: "13px", padding: "0 7px" },
  lg: { minWidth: "32px", height: "32px", fontSize: "15px", padding: "0 9px" },
};

export function Kbd({ children, size = "md" }: KbdProps) {
  const s = sizeStyles[size];

  return (
    <kbd
      style={{
        display: "inline-flex",
        alignItems: "center",
        justifyContent: "center",
        minWidth: s.minWidth,
        height: s.height,
        padding: s.padding,
        fontSize: s.fontSize,
        fontFamily:
          '"SF Mono", "Roboto Mono", ui-monospace, Menlo, Consolas, monospace',
        fontWeight: 500,
        lineHeight: 1,
        // color: "#3f3f46",
        color: "white",
        // background: "linear-gradient(180deg, #ffffff 0%, #f4f4f5 100%)",
        background: "linear-gradient(180deg, #3f3f46 0%, #27272a 100%)",
        border: "1px solid #d4d4d8",
        borderColor: "#52525b",
        borderBottomWidth: "2px",
        borderRadius: "6px",
        boxShadow:
          "0 1px 0 rgba(0,0,0,0.04), inset 0 1px 0 rgba(255,255,255,0.8)",
        userSelect: "none",
        whiteSpace: "nowrap",
      }}
    >
      {children}
    </kbd>
  );
}

// Usage examples:
// <Kbd>K</Kbd>
// <div style={{ display: "flex", gap: 4, alignItems: "center" }}>
//   <Kbd>⌘</Kbd><Kbd>K</Kbd>
// </div>
// <Kbd size="sm">Esc</Kbd>