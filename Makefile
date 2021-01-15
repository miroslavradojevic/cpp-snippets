.PHONY: clean All

All:
	@echo "----------Building project:[ General - Debug ]----------"
	@cd "General" && "$(MAKE)" -f  "General.mk"
clean:
	@echo "----------Cleaning project:[ General - Debug ]----------"
	@cd "General" && "$(MAKE)" -f  "General.mk" clean
